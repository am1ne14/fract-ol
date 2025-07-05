#!/usr/bin/env python3
"""
Game Analysis Tool

Analyzes patterns across multiple CTF games to identify similarities
in .so files, input patterns, and optimization opportunities.
"""

import os
import sys
import hashlib
import subprocess
from pathlib import Path
from typing import Dict, List, Set, Tuple, Optional
import json
import re
from collections import defaultdict, Counter


class SOFileAnalyzer:
    """Analyzes .so files for patterns and similarities."""
    
    def __init__(self):
        self.file_hashes = {}
        self.file_sizes = {}
        self.symbols = {}
        self.dependencies = {}
    
    def analyze_so_file(self, so_path: str) -> Dict:
        """
        Analyze a single .so file for key characteristics.
        
        Returns:
            Dictionary with analysis results
        """
        if not os.path.exists(so_path):
            return {"error": "File not found"}
        
        analysis = {
            "path": so_path,
            "size": os.path.getsize(so_path),
            "hash": self.get_file_hash(so_path),
            "symbols": self.extract_symbols(so_path),
            "dependencies": self.get_dependencies(so_path),
            "strings": self.extract_strings(so_path)
        }
        
        return analysis
    
    def get_file_hash(self, file_path: str) -> str:
        """Calculate SHA256 hash of file."""
        hasher = hashlib.sha256()
        try:
            with open(file_path, 'rb') as f:
                for chunk in iter(lambda: f.read(4096), b""):
                    hasher.update(chunk)
            return hasher.hexdigest()
        except Exception:
            return "error"
    
    def extract_symbols(self, so_path: str) -> List[str]:
        """Extract symbol names from .so file using nm."""
        try:
            result = subprocess.run(['nm', '-D', so_path], 
                                  capture_output=True, text=True, timeout=10)
            if result.returncode == 0:
                symbols = []
                for line in result.stdout.strip().split('\n'):
                    if line and ' ' in line:
                        parts = line.split()
                        if len(parts) >= 3:
                            symbols.append(parts[-1])
                return symbols
        except Exception:
            pass
        return []
    
    def get_dependencies(self, so_path: str) -> List[str]:
        """Get library dependencies using ldd."""
        try:
            result = subprocess.run(['ldd', so_path], 
                                  capture_output=True, text=True, timeout=10)
            if result.returncode == 0:
                deps = []
                for line in result.stdout.strip().split('\n'):
                    if '=>' in line:
                        dep = line.split('=>')[0].strip()
                        deps.append(dep)
                return deps
        except Exception:
            pass
        return []
    
    def extract_strings(self, so_path: str) -> List[str]:
        """Extract printable strings from .so file."""
        try:
            result = subprocess.run(['strings', so_path], 
                                  capture_output=True, text=True, timeout=10)
            if result.returncode == 0:
                strings = result.stdout.strip().split('\n')
                # Filter interesting strings
                interesting = []
                for s in strings:
                    if len(s) >= 4 and any(c.isalpha() for c in s):
                        interesting.append(s)
                return interesting[:50]  # Limit to first 50
        except Exception:
            pass
        return []


class GamePatternAnalyzer:
    """Analyzes patterns across multiple games."""
    
    def __init__(self):
        self.so_analyzer = SOFileAnalyzer()
        self.game_data = {}
        self.patterns = defaultdict(list)
    
    def analyze_game_directory(self, games_dir: str) -> Dict:
        """
        Analyze all games in a directory structure.
        
        Expected structure:
        games_dir/
        ├── game1/
        │   ├── game_executable
        │   └── *.so files
        ├── game2/
        │   ├── game_executable  
        │   └── *.so files
        ...
        """
        if not os.path.exists(games_dir):
            print(f"Games directory not found: {games_dir}")
            return {}
        
        games_path = Path(games_dir)
        game_dirs = [d for d in games_path.iterdir() if d.is_dir()]
        
        print(f"Found {len(game_dirs)} game directories")
        
        analysis_results = {
            "total_games": len(game_dirs),
            "games": {},
            "patterns": {},
            "statistics": {}
        }
        
        for i, game_dir in enumerate(sorted(game_dirs)):
            game_name = game_dir.name
            print(f"Analyzing game {i+1}/{len(game_dirs)}: {game_name}")
            
            game_analysis = self.analyze_single_game(str(game_dir))
            analysis_results["games"][game_name] = game_analysis
            
            # Track patterns
            self.track_patterns(game_name, game_analysis)
        
        # Generate pattern analysis
        analysis_results["patterns"] = self.generate_pattern_analysis()
        analysis_results["statistics"] = self.generate_statistics()
        
        return analysis_results
    
    def analyze_single_game(self, game_dir: str) -> Dict:
        """Analyze a single game directory."""
        game_path = Path(game_dir)
        
        # Find executable
        executables = list(game_path.glob("game*")) + list(game_path.glob("*"))
        executable = None
        for exe in executables:
            if exe.is_file() and os.access(exe, os.X_OK):
                executable = str(exe)
                break
        
        # Find .so files
        so_files = list(game_path.glob("*.so"))
        
        analysis = {
            "directory": str(game_path),
            "executable": executable,
            "so_files": [str(so) for so in so_files],
            "so_count": len(so_files),
            "so_analysis": {}
        }
        
        # Analyze each .so file
        for so_file in so_files:
            so_analysis = self.so_analyzer.analyze_so_file(str(so_file))
            analysis["so_analysis"][so_file.name] = so_analysis
        
        return analysis
    
    def track_patterns(self, game_name: str, analysis: Dict):
        """Track patterns across games for later analysis."""
        # SO file count pattern
        so_count = analysis.get("so_count", 0)
        self.patterns["so_counts"].append((game_name, so_count))
        
        # SO file name patterns
        for so_file in analysis.get("so_files", []):
            so_name = Path(so_file).name
            self.patterns["so_names"].append((game_name, so_name))
        
        # Hash patterns (identical files)
        for so_name, so_data in analysis.get("so_analysis", {}).items():
            if "hash" in so_data:
                self.patterns["so_hashes"].append((game_name, so_name, so_data["hash"]))
        
        # Symbol patterns
        for so_name, so_data in analysis.get("so_analysis", {}).items():
            for symbol in so_data.get("symbols", []):
                self.patterns["symbols"].append((game_name, so_name, symbol))
    
    def generate_pattern_analysis(self) -> Dict:
        """Generate analysis of discovered patterns."""
        patterns = {}
        
        # SO count distribution
        so_counts = [count for _, count in self.patterns["so_counts"]]
        patterns["so_count_distribution"] = dict(Counter(so_counts))
        
        # Common SO file names
        so_names = [name for _, name in self.patterns["so_names"]]
        patterns["common_so_names"] = dict(Counter(so_names).most_common(20))
        
        # Identical files (same hash)
        hash_groups = defaultdict(list)
        for game, so_name, hash_val in self.patterns["so_hashes"]:
            if hash_val != "error":
                hash_groups[hash_val].append((game, so_name))
        
        patterns["identical_files"] = {
            h: games for h, games in hash_groups.items() if len(games) > 1
        }
        
        # Common symbols
        symbols = [symbol for _, _, symbol in self.patterns["symbols"]]
        patterns["common_symbols"] = dict(Counter(symbols).most_common(50))
        
        return patterns
    
    def generate_statistics(self) -> Dict:
        """Generate overall statistics."""
        total_games = len(set(game for game, _ in self.patterns["so_counts"]))
        total_so_files = len(self.patterns["so_names"])
        unique_hashes = len(set(h for _, _, h in self.patterns["so_hashes"] if h != "error"))
        
        return {
            "total_games": total_games,
            "total_so_files": total_so_files,
            "unique_so_files": unique_hashes,
            "avg_so_per_game": total_so_files / total_games if total_games > 0 else 0
        }
    
    def find_game_clusters(self) -> Dict:
        """Group games by similar characteristics."""
        clusters = defaultdict(list)
        
        # Cluster by SO file count
        for game_name, so_count in self.patterns["so_counts"]:
            clusters[f"so_count_{so_count}"].append(game_name)
        
        # Cluster by common SO files
        game_so_sets = defaultdict(set)
        for game_name, so_name in self.patterns["so_names"]:
            game_so_sets[game_name].add(so_name)
        
        # Find games with identical SO file sets
        so_signature_groups = defaultdict(list)
        for game_name, so_set in game_so_sets.items():
            signature = tuple(sorted(so_set))
            so_signature_groups[signature].append(game_name)
        
        identical_clusters = {
            f"identical_so_set_{i}": games 
            for i, games in enumerate(so_signature_groups.values()) 
            if len(games) > 1
        }
        
        clusters.update(identical_clusters)
        return dict(clusters)
    
    def suggest_solving_strategy(self, analysis_results: Dict) -> List[str]:
        """Suggest optimal solving strategy based on patterns."""
        suggestions = []
        
        patterns = analysis_results.get("patterns", {})
        stats = analysis_results.get("statistics", {})
        
        # Check for identical files
        identical_files = patterns.get("identical_files", {})
        if identical_files:
            suggestions.append(
                f"Found {len(identical_files)} groups of identical .so files. "
                "Games using the same .so files likely have the same solution."
            )
        
        # Check SO count distribution
        so_distribution = patterns.get("so_count_distribution", {})
        if len(so_distribution) == 1:
            count = list(so_distribution.keys())[0]
            suggestions.append(
                f"All games use exactly {count} .so files. "
                "This suggests a consistent challenge structure."
            )
        
        # Check for common symbols
        common_symbols = patterns.get("common_symbols", {})
        if common_symbols:
            top_symbol = list(common_symbols.keys())[0]
            suggestions.append(
                f"Most common symbol: '{top_symbol}' - "
                "focus on this function in reverse engineering."
            )
        
        # Suggest parallelization strategy
        total_games = stats.get("total_games", 0)
        if total_games > 100:
            suggestions.append(
                f"With {total_games} games, parallel processing is recommended. "
                "Group similar games together for batch processing."
            )
        
        return suggestions


def main():
    """Command line interface for game analysis."""
    if len(sys.argv) < 2:
        print("Usage: python analyze_games.py <games_directory> [output_file]")
        print("Example: python analyze_games.py ./ctf_games analysis.json")
        sys.exit(1)
    
    games_dir = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "game_analysis.json"
    
    analyzer = GamePatternAnalyzer()
    
    print("Starting game analysis...")
    results = analyzer.analyze_game_directory(games_dir)
    
    # Add clustering analysis
    print("Generating game clusters...")
    clusters = analyzer.find_game_clusters()
    results["clusters"] = clusters
    
    # Generate solving suggestions
    print("Generating solving strategy suggestions...")
    suggestions = analyzer.suggest_solving_strategy(results)
    results["solving_suggestions"] = suggestions
    
    # Save results
    with open(output_file, 'w') as f:
        json.dump(results, f, indent=2)
    
    print(f"\nAnalysis complete! Results saved to {output_file}")
    
    # Print summary
    stats = results.get("statistics", {})
    patterns = results.get("patterns", {})
    
    print(f"\n=== ANALYSIS SUMMARY ===")
    print(f"Total games: {stats.get('total_games', 0)}")
    print(f"Total .so files: {stats.get('total_so_files', 0)}")
    print(f"Unique .so files: {stats.get('unique_so_files', 0)}")
    print(f"Average .so files per game: {stats.get('avg_so_per_game', 0):.2f}")
    
    print(f"\n=== KEY PATTERNS ===")
    so_distribution = patterns.get("so_count_distribution", {})
    print(f"SO file count distribution: {so_distribution}")
    
    identical_files = patterns.get("identical_files", {})
    print(f"Groups of identical files: {len(identical_files)}")
    
    print(f"\n=== SOLVING SUGGESTIONS ===")
    for suggestion in suggestions:
        print(f"• {suggestion}")
    
    print(f"\n=== CLUSTERS ===")
    for cluster_name, games in clusters.items():
        if len(games) > 1:
            print(f"{cluster_name}: {len(games)} games")


if __name__ == "__main__":
    main()