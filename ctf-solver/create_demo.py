#!/usr/bin/env python3
"""
Demo script that creates a mock CTF challenge structure for testing the solver tools.
"""

import os
import struct
import json
from pathlib import Path


def create_mock_game(game_dir: Path, game_id: int):
    """Create a mock game executable and .so files."""
    game_dir.mkdir(parents=True, exist_ok=True)
    
    # Create a mock executable
    exe_path = game_dir / "game"
    exe_content = f"""#!/bin/bash
# Mock game {game_id}
read -n 8 input
if [ "$input" == "test{game_id:04d}" ]; then
    echo "You win!"
    # Output some mock binary data (JPG chunk)
    echo -ne "\\xFF\\xD8\\x00\\x{game_id:02x}"
else
    echo "Try again!"
fi
""".encode()
    
    with open(exe_path, 'wb') as f:
        f.write(exe_content)
    os.chmod(exe_path, 0o755)
    
    # Create mock .so files
    for i in range(2):
        so_path = game_dir / f"lib{i+1}.so"
        # Simple ELF header for .so file
        elf_header = b'\x7fELF\x02\x01\x01\x00' + b'\x00' * 8
        so_content = elf_header + f"mock_so_game_{game_id}_lib_{i+1}".encode() + b'\x00' * 100
        
        with open(so_path, 'wb') as f:
            f.write(so_content)


def create_mock_results():
    """Create mock solving results for testing JPG assembly."""
    results = {
        "metadata": {
            "total_games": 5,
            "successful_games": 5,
            "timestamp": 1234567890,
            "max_workers": 1,
            "timeout_per_game": 30
        },
        "results": {},
        "successful_outputs": {}
    }
    
    # Create results for 5 games
    for i in range(1, 6):
        game_id = str(i)
        results["results"][game_id] = {
            "game_id": i,
            "game_path": f"./example/game{i}/game",
            "success": True,
            "solution": f"test{i:04d}".encode().hex(),
            "output": f"You win!\nBinary data chunk {i}",
            "error": None,
            "solve_time": 1.5
        }
        
        # Mock JPG chunks for assembly testing
        if i == 1:
            output = "You win!\nffd800ff"  # JPG SOI + data
        elif i == 5:
            output = "You win!\nffd900ff"  # JPG EOI + data  
        else:
            output = f"You win!\n{i:02x}{i:02x}{i:02x}{i:02x}"  # Random data
        
        results["successful_outputs"][game_id] = output
    
    return results


def main():
    """Create demo environment."""
    print("Creating mock CTF challenge environment...")
    
    # Create mock game directories
    example_dir = Path("example")
    for i in range(1, 6):
        game_dir = example_dir / f"game{i}"
        create_mock_game(game_dir, i)
        print(f"Created game{i}")
    
    # Create mock results file
    results = create_mock_results()
    with open("example_results.json", 'w') as f:
        json.dump(results, f, indent=2)
    print("Created example_results.json")
    
    print("\nDemo environment ready!")
    print("You can now test:")
    print("1. python analyze_games.py example")
    print("2. python solve_single.py example/game1/game")
    print("3. python assemble_jpg.py example_results.json --output demo.jpg")


if __name__ == "__main__":
    main()