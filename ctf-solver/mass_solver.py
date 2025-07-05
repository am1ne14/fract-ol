#!/usr/bin/env python3
"""
Mass Solver for CTF Challenge

Automated solver that processes all 11,424 games in parallel,
collects successful outputs, and handles errors gracefully.
"""

import os
import sys
import json
import time
import logging
from pathlib import Path
from typing import Dict, List, Tuple, Optional
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed
from dataclasses import dataclass
import threading
import queue
import signal
from contextlib import contextmanager

from solve_single import GameSolver
from analyze_games import GamePatternAnalyzer


@dataclass
class GameResult:
    """Result from solving a single game."""
    game_id: int
    game_path: str
    success: bool
    solution: Optional[bytes]
    output: str
    error: Optional[str]
    solve_time: float


class ProgressTracker:
    """Thread-safe progress tracking."""
    
    def __init__(self, total_games: int):
        self.total_games = total_games
        self.completed = 0
        self.succeeded = 0
        self.failed = 0
        self.lock = threading.Lock()
        self.start_time = time.time()
    
    def update(self, success: bool):
        with self.lock:
            self.completed += 1
            if success:
                self.succeeded += 1
            else:
                self.failed += 1
    
    def get_status(self) -> Dict:
        with self.lock:
            elapsed = time.time() - self.start_time
            rate = self.completed / elapsed if elapsed > 0 else 0
            eta = (self.total_games - self.completed) / rate if rate > 0 else 0
            
            return {
                "completed": self.completed,
                "succeeded": self.succeeded,
                "failed": self.failed,
                "total": self.total_games,
                "progress_pct": (self.completed / self.total_games) * 100,
                "elapsed_time": elapsed,
                "rate_per_sec": rate,
                "eta_seconds": eta
            }
    
    def print_status(self):
        status = self.get_status()
        print(f"\rProgress: {status['completed']}/{status['total']} "
              f"({status['progress_pct']:.1f}%) | "
              f"Success: {status['succeeded']} | "
              f"Failed: {status['failed']} | "
              f"Rate: {status['rate_per_sec']:.1f}/s | "
              f"ETA: {status['eta_seconds']:.0f}s", end="", flush=True)


class MassSolver:
    """Mass solver for all CTF games."""
    
    def __init__(self, games_dir: str, max_workers: int = 8, 
                 timeout_per_game: int = 30):
        self.games_dir = Path(games_dir)
        self.max_workers = max_workers
        self.timeout_per_game = timeout_per_game
        self.results = {}
        self.successful_outputs = {}
        
        # Setup logging
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s - %(levelname)s - %(message)s',
            handlers=[
                logging.FileHandler('mass_solver.log'),
                logging.StreamHandler()
            ]
        )
        self.logger = logging.getLogger(__name__)
        
        # Load game analysis if available
        self.game_analysis = None
        if os.path.exists("game_analysis.json"):
            with open("game_analysis.json", 'r') as f:
                self.game_analysis = json.load(f)
            self.logger.info("Loaded game analysis data")
    
    def discover_games(self) -> List[Tuple[int, str]]:
        """
        Discover all games in the directory.
        
        Returns:
            List of (game_id, game_path) tuples
        """
        games = []
        
        if not self.games_dir.exists():
            self.logger.error(f"Games directory not found: {self.games_dir}")
            return games
        
        # Look for numbered game directories (game1, game2, etc.)
        for item in self.games_dir.iterdir():
            if item.is_dir():
                # Extract game number from directory name
                game_name = item.name
                game_id = None
                
                # Try to extract number from name
                import re
                match = re.search(r'(\d+)', game_name)
                if match:
                    game_id = int(match.group(1))
                else:
                    # Fallback: use directory index
                    game_id = len(games) + 1
                
                # Find executable in directory
                executable = self.find_game_executable(item)
                if executable:
                    games.append((game_id, str(executable)))
                else:
                    self.logger.warning(f"No executable found in {item}")
        
        # Sort by game ID
        games.sort(key=lambda x: x[0])
        self.logger.info(f"Discovered {len(games)} games")
        return games
    
    def find_game_executable(self, game_dir: Path) -> Optional[str]:
        """Find the main executable in a game directory."""
        # Common executable names
        candidates = [
            "game", "main", "challenge", "ctf", 
            game_dir.name,  # Directory name itself
        ]
        
        # Check for exact matches first
        for candidate in candidates:
            exe_path = game_dir / candidate
            if exe_path.is_file() and os.access(exe_path, os.X_OK):
                return str(exe_path)
        
        # Look for any executable file
        for item in game_dir.iterdir():
            if item.is_file() and os.access(item, os.X_OK):
                # Check if it's a binary (not a script)
                try:
                    with open(item, 'rb') as f:
                        header = f.read(4)
                        if header.startswith(b'\x7fELF'):  # ELF binary
                            return str(item)
                except Exception:
                    pass
        
        return None
    
    @contextmanager
    def timeout_handler(self, seconds: int):
        """Context manager for handling timeouts."""
        def timeout_signal_handler(signum, frame):
            raise TimeoutError(f"Operation timed out after {seconds} seconds")
        
        # Set the signal handler and a {seconds} second alarm
        old_handler = signal.signal(signal.SIGALRM, timeout_signal_handler)
        signal.alarm(seconds)
        try:
            yield
        finally:
            signal.alarm(0)  # Disable the alarm
            signal.signal(signal.SIGALRM, old_handler)
    
    def solve_single_game(self, game_id: int, game_path: str) -> GameResult:
        """Solve a single game with timeout and error handling."""
        start_time = time.time()
        
        try:
            # Get .so files for this game
            game_dir = Path(game_path).parent
            so_files = list(game_dir.glob("*.so"))
            
            # Use analysis data if available for optimization
            solver_kwargs = {}
            if self.game_analysis:
                # Apply optimizations based on analysis
                game_name = game_dir.name
                if game_name in self.game_analysis.get("games", {}):
                    # Could add specific optimizations here
                    pass
            
            solver = GameSolver(game_path, [str(so) for so in so_files])
            
            # Try mathematical solution first (faster)
            solution = solver.brute_force_solve(max_attempts=10000)
            if solution:
                # Verify with actual game
                success, output = solver.run_game(solution)
                if success:
                    solve_time = time.time() - start_time
                    return GameResult(
                        game_id=game_id,
                        game_path=game_path,
                        success=True,
                        solution=solution,
                        output=output,
                        error=None,
                        solve_time=solve_time
                    )
            
            # Fall back to direct game execution
            result = solver.solve_with_game(max_attempts=5000)
            if result:
                solution, output = result
                solve_time = time.time() - start_time
                return GameResult(
                    game_id=game_id,
                    game_path=game_path,
                    success=True,
                    solution=solution,
                    output=output,
                    error=None,
                    solve_time=solve_time
                )
            
            # No solution found
            solve_time = time.time() - start_time
            return GameResult(
                game_id=game_id,
                game_path=game_path,
                success=False,
                solution=None,
                output="",
                error="No solution found",
                solve_time=solve_time
            )
        
        except Exception as e:
            solve_time = time.time() - start_time
            return GameResult(
                game_id=game_id,
                game_path=game_path,
                success=False,
                solution=None,
                output="",
                error=str(e),
                solve_time=solve_time
            )
    
    def solve_all_games(self, use_processes: bool = False) -> Dict[int, GameResult]:
        """
        Solve all games using parallel processing.
        
        Args:
            use_processes: Use ProcessPoolExecutor instead of ThreadPoolExecutor
            
        Returns:
            Dictionary mapping game_id to GameResult
        """
        games = self.discover_games()
        if not games:
            self.logger.error("No games found to solve")
            return {}
        
        self.logger.info(f"Starting mass solve of {len(games)} games with {self.max_workers} workers")
        
        progress = ProgressTracker(len(games))
        results = {}
        
        # Choose executor type
        executor_class = ProcessPoolExecutor if use_processes else ThreadPoolExecutor
        
        with executor_class(max_workers=self.max_workers) as executor:
            # Submit all jobs
            future_to_game = {
                executor.submit(self.solve_single_game, game_id, game_path): (game_id, game_path)
                for game_id, game_path in games
            }
            
            # Process completed jobs
            try:
                for future in as_completed(future_to_game):
                    game_id, game_path = future_to_game[future]
                    
                    try:
                        result = future.result()
                        results[game_id] = result
                        
                        if result.success:
                            self.successful_outputs[game_id] = result.output
                            self.logger.info(f"Game {game_id} solved: {result.solution.hex()}")
                        else:
                            self.logger.warning(f"Game {game_id} failed: {result.error}")
                        
                        progress.update(result.success)
                        
                        # Print progress every 10 completions
                        if progress.completed % 10 == 0:
                            progress.print_status()
                    
                    except Exception as e:
                        self.logger.error(f"Error processing game {game_id}: {e}")
                        progress.update(False)
            
            except KeyboardInterrupt:
                self.logger.info("Interrupted by user, saving partial results...")
                # Cancel remaining futures
                for future in future_to_game:
                    future.cancel()
        
        # Final progress update
        progress.print_status()
        print()  # New line after progress
        
        final_status = progress.get_status()
        self.logger.info(f"Mass solve complete: {final_status['succeeded']}/{final_status['total']} games solved")
        
        self.results = results
        return results
    
    def save_results(self, output_file: str = "mass_solve_results.json"):
        """Save solving results to file."""
        # Convert results to serializable format
        serializable_results = {}
        for game_id, result in self.results.items():
            serializable_results[str(game_id)] = {
                "game_id": result.game_id,
                "game_path": result.game_path,
                "success": result.success,
                "solution": result.solution.hex() if result.solution else None,
                "output": result.output,
                "error": result.error,
                "solve_time": result.solve_time
            }
        
        save_data = {
            "metadata": {
                "total_games": len(self.results),
                "successful_games": len(self.successful_outputs),
                "timestamp": time.time(),
                "max_workers": self.max_workers,
                "timeout_per_game": self.timeout_per_game
            },
            "results": serializable_results,
            "successful_outputs": self.successful_outputs
        }
        
        with open(output_file, 'w') as f:
            json.dump(save_data, f, indent=2)
        
        self.logger.info(f"Results saved to {output_file}")
    
    def load_results(self, input_file: str = "mass_solve_results.json") -> bool:
        """Load previously saved results."""
        try:
            with open(input_file, 'r') as f:
                data = json.load(f)
            
            # Convert back to GameResult objects
            self.results = {}
            for game_id_str, result_data in data["results"].items():
                game_id = int(game_id_str)
                solution = None
                if result_data["solution"]:
                    solution = bytes.fromhex(result_data["solution"])
                
                self.results[game_id] = GameResult(
                    game_id=result_data["game_id"],
                    game_path=result_data["game_path"],
                    success=result_data["success"],
                    solution=solution,
                    output=result_data["output"],
                    error=result_data["error"],
                    solve_time=result_data["solve_time"]
                )
            
            self.successful_outputs = data["successful_outputs"]
            self.logger.info(f"Loaded {len(self.results)} results from {input_file}")
            return True
        
        except Exception as e:
            self.logger.error(f"Failed to load results: {e}")
            return False
    
    def get_statistics(self) -> Dict:
        """Get solving statistics."""
        if not self.results:
            return {}
        
        successful = [r for r in self.results.values() if r.success]
        failed = [r for r in self.results.values() if not r.success]
        
        solve_times = [r.solve_time for r in successful]
        
        stats = {
            "total_games": len(self.results),
            "successful": len(successful),
            "failed": len(failed),
            "success_rate": len(successful) / len(self.results) * 100,
            "avg_solve_time": sum(solve_times) / len(solve_times) if solve_times else 0,
            "min_solve_time": min(solve_times) if solve_times else 0,
            "max_solve_time": max(solve_times) if solve_times else 0,
            "total_solve_time": sum(solve_times)
        }
        
        # Error analysis
        error_counts = {}
        for result in failed:
            error = result.error or "Unknown"
            error_counts[error] = error_counts.get(error, 0) + 1
        
        stats["error_breakdown"] = error_counts
        
        return stats


def main():
    """Command line interface for mass solver."""
    if len(sys.argv) < 2:
        print("Usage: python mass_solver.py <games_directory> [options]")
        print("Options:")
        print("  --workers N      Number of parallel workers (default: 8)")
        print("  --timeout N      Timeout per game in seconds (default: 30)")
        print("  --processes      Use processes instead of threads")
        print("  --load FILE      Load previous results instead of solving")
        print("  --output FILE    Output file for results (default: mass_solve_results.json)")
        sys.exit(1)
    
    games_dir = sys.argv[1]
    max_workers = 8
    timeout = 30
    use_processes = False
    load_file = None
    output_file = "mass_solve_results.json"
    
    # Parse command line options
    i = 2
    while i < len(sys.argv):
        if sys.argv[i] == "--workers" and i + 1 < len(sys.argv):
            max_workers = int(sys.argv[i + 1])
            i += 2
        elif sys.argv[i] == "--timeout" and i + 1 < len(sys.argv):
            timeout = int(sys.argv[i + 1])
            i += 2
        elif sys.argv[i] == "--processes":
            use_processes = True
            i += 1
        elif sys.argv[i] == "--load" and i + 1 < len(sys.argv):
            load_file = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == "--output" and i + 1 < len(sys.argv):
            output_file = sys.argv[i + 1]
            i += 2
        else:
            print(f"Unknown option: {sys.argv[i]}")
            sys.exit(1)
    
    solver = MassSolver(games_dir, max_workers, timeout)
    
    if load_file:
        # Load previous results
        if solver.load_results(load_file):
            print("Loaded previous results")
        else:
            sys.exit(1)
    else:
        # Solve all games
        print(f"Starting mass solve with {max_workers} workers, {timeout}s timeout per game")
        print(f"Using {'processes' if use_processes else 'threads'} for parallelization")
        
        try:
            results = solver.solve_all_games(use_processes=use_processes)
            solver.save_results(output_file)
        except KeyboardInterrupt:
            print("\nInterrupted by user")
            solver.save_results(f"partial_{output_file}")
            sys.exit(1)
    
    # Print statistics
    stats = solver.get_statistics()
    print(f"\n=== SOLVING STATISTICS ===")
    print(f"Total games: {stats['total_games']}")
    print(f"Successful: {stats['successful']}")
    print(f"Failed: {stats['failed']}")
    print(f"Success rate: {stats['success_rate']:.1f}%")
    print(f"Average solve time: {stats['avg_solve_time']:.2f}s")
    print(f"Total time spent solving: {stats['total_solve_time']:.1f}s")
    
    if stats.get('error_breakdown'):
        print(f"\n=== ERROR BREAKDOWN ===")
        for error, count in stats['error_breakdown'].items():
            print(f"{error}: {count}")
    
    print(f"\nResults saved to: {output_file}")
    print("Ready for JPG assembly!")


if __name__ == "__main__":
    main()