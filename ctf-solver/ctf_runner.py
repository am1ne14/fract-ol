#!/usr/bin/env python3
"""
CTF Solver Toolkit Runner

Unified interface for running the complete CTF solving workflow.
Provides guided execution of all solver components.
"""

import os
import sys
import json
import argparse
from pathlib import Path
from typing import Optional

from solve_single import GameSolver
from analyze_games import GamePatternAnalyzer
from mass_solver import MassSolver
from assemble_jpg import JPGAssembler


class CTFSolverRunner:
    """Main runner for CTF solver toolkit."""
    
    def __init__(self, games_dir: str, output_dir: str = "output"):
        self.games_dir = Path(games_dir)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)
        
        # Output file paths
        self.analysis_file = self.output_dir / "game_analysis.json"
        self.results_file = self.output_dir / "solve_results.json"
        self.final_image = self.output_dir / "ctf_solution.jpg"
        
    def run_complete_workflow(self, workers: int = 8, timeout: int = 30,
                            skip_analysis: bool = False) -> bool:
        """
        Run the complete CTF solving workflow.
        
        Args:
            workers: Number of parallel workers for mass solving
            timeout: Timeout per game in seconds
            skip_analysis: Skip pattern analysis phase
            
        Returns:
            True if successful, False otherwise
        """
        print("🚀 Starting CTF Auto-Rev Challenge Solver")
        print(f"Games directory: {self.games_dir}")
        print(f"Output directory: {self.output_dir}")
        print()
        
        # Step 1: Analyze games (optional)
        if not skip_analysis:
            print("📊 Step 1: Analyzing game patterns...")
            if not self.run_analysis():
                print("❌ Analysis failed, continuing without optimization")
            else:
                print("✅ Analysis complete")
            print()
        
        # Step 2: Mass solve all games
        print(f"🔧 Step 2: Solving all games (workers: {workers}, timeout: {timeout}s)...")
        if not self.run_mass_solve(workers, timeout):
            print("❌ Mass solving failed")
            return False
        print("✅ Mass solving complete")
        print()
        
        # Step 3: Assemble JPG
        print("🖼️  Step 3: Assembling final JPG image...")
        if not self.run_assembly():
            print("❌ JPG assembly failed")
            return False
        print("✅ JPG assembly complete")
        print()
        
        print(f"🎉 CTF challenge solved! Final image: {self.final_image}")
        return True
    
    def run_analysis(self) -> bool:
        """Run game pattern analysis."""
        try:
            analyzer = GamePatternAnalyzer()
            results = analyzer.analyze_game_directory(str(self.games_dir))
            
            if not results or results.get("total_games", 0) == 0:
                return False
            
            # Save analysis results
            with open(self.analysis_file, 'w') as f:
                json.dump(results, f, indent=2)
            
            # Print summary
            stats = results.get("statistics", {})
            print(f"   Found {stats.get('total_games', 0)} games")
            print(f"   Average .so files per game: {stats.get('avg_so_per_game', 0):.1f}")
            
            suggestions = results.get("solving_suggestions", [])
            if suggestions:
                print("   Key insights:")
                for suggestion in suggestions[:3]:
                    print(f"     • {suggestion}")
            
            return True
        
        except Exception as e:
            print(f"   Error during analysis: {e}")
            return False
    
    def run_mass_solve(self, workers: int, timeout: int) -> bool:
        """Run mass solving of all games."""
        try:
            solver = MassSolver(str(self.games_dir), workers, timeout)
            results = solver.solve_all_games()
            
            if not results:
                return False
            
            # Save results
            solver.save_results(str(self.results_file))
            
            # Print summary
            stats = solver.get_statistics()
            print(f"   Solved {stats['successful']}/{stats['total_games']} games")
            print(f"   Success rate: {stats['success_rate']:.1f}%")
            print(f"   Average solve time: {stats['avg_solve_time']:.2f}s")
            
            return stats['successful'] > 0
        
        except Exception as e:
            print(f"   Error during mass solving: {e}")
            return False
    
    def run_assembly(self) -> bool:
        """Run JPG assembly from solving results."""
        try:
            if not self.results_file.exists():
                print(f"   Results file not found: {self.results_file}")
                return False
            
            assembler = JPGAssembler(str(self.results_file))
            success = assembler.assemble_jpg(str(self.final_image))
            
            if success:
                size = self.final_image.stat().st_size
                print(f"   Created {self.final_image} ({size} bytes)")
            
            return success
        
        except Exception as e:
            print(f"   Error during JPG assembly: {e}")
            return False
    
    def test_single_game(self, game_path: str) -> bool:
        """Test solver on a single game."""
        print(f"🧪 Testing single game: {game_path}")
        
        try:
            solver = GameSolver(game_path)
            result = solver.solve_with_game(max_attempts=1000)
            
            if result:
                solution, output = result
                print(f"✅ Solution found: {solution.hex()}")
                print(f"   Output: {output[:100]}...")
                return True
            else:
                print("❌ No solution found")
                return False
        
        except Exception as e:
            print(f"❌ Error: {e}")
            return False
    
    def show_status(self) -> None:
        """Show current status of solving progress."""
        print("📈 CTF Solver Status")
        print("=" * 50)
        
        # Check analysis
        if self.analysis_file.exists():
            with open(self.analysis_file, 'r') as f:
                analysis = json.load(f)
            stats = analysis.get("statistics", {})
            print(f"✅ Analysis: {stats.get('total_games', 0)} games analyzed")
        else:
            print("❌ Analysis: Not completed")
        
        # Check solving results
        if self.results_file.exists():
            with open(self.results_file, 'r') as f:
                results = json.load(f)
            metadata = results.get("metadata", {})
            successful = metadata.get("successful_games", 0)
            total = metadata.get("total_games", 0)
            print(f"✅ Solving: {successful}/{total} games solved")
        else:
            print("❌ Solving: Not completed")
        
        # Check final image
        if self.final_image.exists():
            size = self.final_image.stat().st_size
            print(f"✅ Assembly: Final image created ({size} bytes)")
        else:
            print("❌ Assembly: Not completed")


def main():
    """Command line interface for CTF solver runner."""
    parser = argparse.ArgumentParser(
        description="CTF Auto-Rev Challenge Solver",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Run complete workflow
  python ctf_runner.py ./ctf_games --workers 16 --timeout 60
  
  # Test single game
  python ctf_runner.py ./ctf_games --test-game ./ctf_games/game1/game
  
  # Show current status
  python ctf_runner.py ./ctf_games --status
  
  # Run specific steps
  python ctf_runner.py ./ctf_games --analyze-only
  python ctf_runner.py ./ctf_games --solve-only --workers 8
  python ctf_runner.py ./ctf_games --assemble-only
        """
    )
    
    parser.add_argument("games_dir", help="Directory containing CTF games")
    parser.add_argument("--workers", type=int, default=8, help="Number of parallel workers")
    parser.add_argument("--timeout", type=int, default=30, help="Timeout per game (seconds)")
    parser.add_argument("--output-dir", default="output", help="Output directory")
    
    # Action options
    parser.add_argument("--test-game", help="Test solver on single game")
    parser.add_argument("--status", action="store_true", help="Show current status")
    parser.add_argument("--analyze-only", action="store_true", help="Only run analysis")
    parser.add_argument("--solve-only", action="store_true", help="Only run mass solving")
    parser.add_argument("--assemble-only", action="store_true", help="Only run JPG assembly")
    parser.add_argument("--skip-analysis", action="store_true", help="Skip analysis phase")
    
    args = parser.parse_args()
    
    # Create runner
    runner = CTFSolverRunner(args.games_dir, args.output_dir)
    
    # Handle different modes
    if args.test_game:
        success = runner.test_single_game(args.test_game)
        sys.exit(0 if success else 1)
    
    elif args.status:
        runner.show_status()
        sys.exit(0)
    
    elif args.analyze_only:
        success = runner.run_analysis()
        sys.exit(0 if success else 1)
    
    elif args.solve_only:
        success = runner.run_mass_solve(args.workers, args.timeout)
        sys.exit(0 if success else 1)
    
    elif args.assemble_only:
        success = runner.run_assembly()
        sys.exit(0 if success else 1)
    
    else:
        # Run complete workflow
        success = runner.run_complete_workflow(
            workers=args.workers,
            timeout=args.timeout,
            skip_analysis=args.skip_analysis
        )
        sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()