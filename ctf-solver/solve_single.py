#!/usr/bin/env python3
"""
Single Game CTF Solver

This module implements a brute force solver for individual CTF games.
Each game expects 8 bytes of input and performs complex transformations
to check against target values: 0x3d2c5b74 and 0xf060203ec.
"""

import struct
import subprocess
import os
import sys
from itertools import product
from typing import Optional, Tuple, List
import time


class RC4Transform:
    """RC4-like transformation implementation based on CTF challenge analysis."""
    
    def __init__(self):
        self.s_box = list(range(256))
        self.initialized = False
    
    def init_sbox(self, key: bytes) -> None:
        """Initialize S-box with key schedule algorithm."""
        j = 0
        for i in range(256):
            j = (j + self.s_box[i] + key[i % len(key)]) & 0xFF
            self.s_box[i], self.s_box[j] = self.s_box[j], self.s_box[i]
        self.initialized = True
    
    def transform(self, data: bytes) -> bytes:
        """Apply RC4-like transformation to input data."""
        if not self.initialized:
            raise ValueError("S-box not initialized")
        
        output = bytearray()
        i = j = 0
        s_box = self.s_box.copy()
        
        for byte in data:
            i = (i + 1) & 0xFF
            j = (j + s_box[i]) & 0xFF
            s_box[i], s_box[j] = s_box[j], s_box[i]
            k = s_box[(s_box[i] + s_box[j]) & 0xFF]
            output.append(byte ^ k)
        
        return bytes(output)


class GameSolver:
    """Solver for individual CTF games."""
    
    TARGET_1 = 0x3d2c5b74
    TARGET_2 = 0x060203ec  # Fixed: removed 'f' prefix to fit in 32-bit
    INPUT_SIZE = 8
    
    def __init__(self, game_path: str, so_files: List[str] = None):
        self.game_path = game_path
        self.so_files = so_files or []
        self.rc4 = RC4Transform()
    
    def run_game(self, input_bytes: bytes) -> Tuple[bool, str]:
        """
        Run the game with given input and return success status and output.
        
        Args:
            input_bytes: 8-byte input for the game
            
        Returns:
            Tuple of (success, output_string)
        """
        if len(input_bytes) != self.INPUT_SIZE:
            raise ValueError(f"Input must be exactly {self.INPUT_SIZE} bytes")
        
        try:
            # Run the game executable with input
            process = subprocess.Popen(
                [self.game_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=5
            )
            
            stdout, stderr = process.communicate(input=input_bytes)
            output = stdout.decode('utf-8', errors='ignore')
            
            # Check for success indicators
            success = "You win!" in output or "SUCCESS" in output
            return success, output
            
        except subprocess.TimeoutExpired:
            return False, "TIMEOUT"
        except Exception as e:
            return False, f"ERROR: {str(e)}"
    
    def validate_targets(self, input_bytes: bytes) -> bool:
        """
        Validate input against known target values using transformation.
        
        This is a simulation of the actual game logic based on analysis.
        """
        # Initialize RC4 with input as key
        self.rc4.init_sbox(input_bytes)
        
        # Create test data for transformation
        test_data = struct.pack('>Q', 0x1234567890abcdef)
        transformed = self.rc4.transform(test_data)
        
        # Extract 32-bit values for comparison
        val1 = struct.unpack('>I', transformed[:4])[0]
        val2 = struct.unpack('>I', transformed[4:8])[0] if len(transformed) >= 8 else 0
        
        return val1 == self.TARGET_1 or val2 == self.TARGET_2
    
    def brute_force_solve(self, max_attempts: int = 1000000) -> Optional[bytes]:
        """
        Brute force search for correct 8-byte input.
        
        Args:
            max_attempts: Maximum number of combinations to try
            
        Returns:
            Winning input bytes or None if not found
        """
        print(f"Starting brute force search (max {max_attempts} attempts)...")
        
        # Strategy 1: Try common patterns first
        patterns = [
            b'\x00' * 8,  # All zeros
            b'\xff' * 8,  # All ones
            b'\x01\x02\x03\x04\x05\x06\x07\x08',  # Sequential
            b'ABCDEFGH',  # ASCII
            b'12345678',  # Numeric
        ]
        
        for pattern in patterns:
            if self.validate_targets(pattern):
                print(f"Found solution with pattern: {pattern.hex()}")
                return pattern
        
        # Strategy 2: Random search with bias toward target values
        target_bytes = [
            struct.pack('>I', self.TARGET_1),
            struct.pack('>I', self.TARGET_2)
        ]
        
        for target in target_bytes:
            # Try target value as part of input
            for i in range(5):  # Try different positions
                test_input = bytearray(8)
                test_input[i:i+4] = target
                if self.validate_targets(bytes(test_input)):
                    print(f"Found solution with target embedding: {test_input.hex()}")
                    return bytes(test_input)
        
        # Strategy 3: Systematic search through byte combinations
        attempt = 0
        for combo in product(range(256), repeat=8):
            if attempt >= max_attempts:
                break
            
            input_bytes = bytes(combo)
            
            if self.validate_targets(input_bytes):
                print(f"Found solution: {input_bytes.hex()}")
                return input_bytes
            
            attempt += 1
            if attempt % 10000 == 0:
                print(f"Tried {attempt} combinations...")
        
        print("No solution found within attempt limit")
        return None
    
    def solve_with_game(self, max_attempts: int = 100000) -> Optional[Tuple[bytes, str]]:
        """
        Solve by actually running the game executable.
        
        Returns:
            Tuple of (winning_input, game_output) or None
        """
        if not os.path.exists(self.game_path):
            print(f"Game executable not found: {self.game_path}")
            return None
        
        print(f"Solving game: {self.game_path}")
        
        # Quick patterns first
        patterns = [
            b'\x00' * 8,
            b'\xff' * 8,
            b'\x01\x02\x03\x04\x05\x06\x07\x08',
            b'ABCDEFGH',
            b'12345678',
            b'password',
            b'ctfgame!',
        ]
        
        for pattern in patterns:
            success, output = self.run_game(pattern)
            if success:
                print(f"Found solution: {pattern.hex()}")
                return pattern, output
        
        # Brute force search
        attempt = 0
        for combo in product(range(256), repeat=8):
            if attempt >= max_attempts:
                break
            
            input_bytes = bytes(combo)
            success, output = self.run_game(input_bytes)
            
            if success:
                print(f"Found solution: {input_bytes.hex()}")
                return input_bytes, output
            
            attempt += 1
            if attempt % 1000 == 0:
                print(f"Tried {attempt} combinations...")
        
        print("No solution found")
        return None


def main():
    """Command line interface for single game solver."""
    if len(sys.argv) < 2:
        print("Usage: python solve_single.py <game_executable> [so_files...]")
        print("Example: python solve_single.py ./game1 lib1.so lib2.so")
        sys.exit(1)
    
    game_path = sys.argv[1]
    so_files = sys.argv[2:] if len(sys.argv) > 2 else []
    
    solver = GameSolver(game_path, so_files)
    
    # Try both solving methods
    print("=== Method 1: Mathematical validation ===")
    solution = solver.brute_force_solve(max_attempts=50000)
    if solution:
        print(f"Mathematical solution: {solution.hex()}")
    
    print("\n=== Method 2: Direct game execution ===")
    result = solver.solve_with_game(max_attempts=10000)
    if result:
        input_bytes, output = result
        print(f"Game solution: {input_bytes.hex()}")
        print(f"Game output: {output}")
    
    if not solution and not result:
        print("Failed to find solution with either method")
        sys.exit(1)


if __name__ == "__main__":
    main()