#!/usr/bin/env python3
"""
JPG Assembly Tool

Assembles the final JPG image from collected game outputs.
Handles proper ordering, validation, and image reconstruction.
"""

import os
import sys
import json
import struct
from pathlib import Path
from typing import Dict, List, Optional, Tuple
import logging


class JPGAssembler:
    """Assembles JPG image from game solving outputs."""
    
    # JPG file signature and markers
    JPG_SOI = b'\xff\xd8'  # Start of Image
    JPG_EOI = b'\xff\xd9'  # End of Image
    JPG_APP0 = b'\xff\xe0'  # JFIF marker
    JPG_SOF0 = b'\xff\xc0'  # Start of Frame (baseline DCT)
    JPG_DHT = b'\xff\xc4'   # Define Huffman Table
    JPG_DQT = b'\xff\xdb'   # Define Quantization Table
    JPG_SOS = b'\xff\xda'   # Start of Scan
    
    def __init__(self, results_file: str):
        self.results_file = results_file
        self.game_outputs = {}
        self.ordered_data = []
        
        # Setup logging
        logging.basicConfig(level=logging.INFO)
        self.logger = logging.getLogger(__name__)
        
        self.load_results()
    
    def load_results(self):
        """Load game solving results from file."""
        if not os.path.exists(self.results_file):
            raise FileNotFoundError(f"Results file not found: {self.results_file}")
        
        with open(self.results_file, 'r') as f:
            data = json.load(f)
        
        self.game_outputs = data.get("successful_outputs", {})
        self.logger.info(f"Loaded outputs from {len(self.game_outputs)} successful games")
        
        if not self.game_outputs:
            raise ValueError("No successful game outputs found in results file")
    
    def extract_binary_data(self, output_text: str) -> Optional[bytes]:
        """
        Extract binary data from game output text.
        
        The game output might contain:
        - "You win!" message followed by binary data
        - Hex-encoded data
        - Base64-encoded data
        - Raw binary data after certain markers
        """
        if not output_text:
            return None
        
        # Method 1: Look for data after "You win!" message
        win_marker = "You win!"
        if win_marker in output_text:
            after_win = output_text.split(win_marker, 1)[1]
            
            # Try to extract hex data
            hex_data = self.extract_hex_data(after_win)
            if hex_data:
                return hex_data
            
            # Try to extract base64 data
            b64_data = self.extract_base64_data(after_win)
            if b64_data:
                return b64_data
            
            # Try to get raw bytes after the message
            if len(after_win) > 0:
                try:
                    return after_win.encode('latin1')
                except:
                    pass
        
        # Method 2: Look for hex patterns in entire output
        hex_data = self.extract_hex_data(output_text)
        if hex_data:
            return hex_data
        
        # Method 3: Look for base64 patterns
        b64_data = self.extract_base64_data(output_text)
        if b64_data:
            return b64_data
        
        # Method 4: Try to interpret entire output as binary
        try:
            return output_text.encode('latin1')
        except:
            return None
    
    def extract_hex_data(self, text: str) -> Optional[bytes]:
        """Extract hex-encoded data from text."""
        import re
        
        # Look for hex patterns (pairs of hex digits)
        hex_pattern = r'([0-9a-fA-F]{2,})'
        matches = re.findall(hex_pattern, text)
        
        for match in matches:
            if len(match) % 2 == 0 and len(match) >= 4:
                try:
                    return bytes.fromhex(match)
                except ValueError:
                    continue
        
        return None
    
    def extract_base64_data(self, text: str) -> Optional[bytes]:
        """Extract base64-encoded data from text."""
        import base64
        import re
        
        # Look for base64 patterns
        b64_pattern = r'([A-Za-z0-9+/]{4,}={0,2})'
        matches = re.findall(b64_pattern, text)
        
        for match in matches:
            if len(match) >= 4:
                try:
                    return base64.b64decode(match)
                except:
                    continue
        
        return None
    
    def order_game_data(self) -> List[Tuple[int, bytes]]:
        """
        Order game data by game ID and extract binary content.
        
        Returns:
            List of (game_id, binary_data) tuples ordered by game_id
        """
        ordered_data = []
        
        for game_id_str, output in self.game_outputs.items():
            try:
                game_id = int(game_id_str)
                binary_data = self.extract_binary_data(output)
                
                if binary_data:
                    ordered_data.append((game_id, binary_data))
                    self.logger.debug(f"Game {game_id}: extracted {len(binary_data)} bytes")
                else:
                    self.logger.warning(f"Game {game_id}: no binary data extracted")
            
            except ValueError:
                self.logger.error(f"Invalid game ID: {game_id_str}")
        
        # Sort by game ID
        ordered_data.sort(key=lambda x: x[0])
        self.logger.info(f"Ordered data from {len(ordered_data)} games")
        
        return ordered_data
    
    def concatenate_data(self, ordered_data: List[Tuple[int, bytes]]) -> bytes:
        """Concatenate all binary data in order."""
        all_data = b''
        
        for game_id, data in ordered_data:
            all_data += data
            self.logger.debug(f"Added {len(data)} bytes from game {game_id}")
        
        self.logger.info(f"Total concatenated data: {len(all_data)} bytes")
        return all_data
    
    def validate_jpg_structure(self, data: bytes) -> Tuple[bool, str]:
        """
        Validate that the data has proper JPG structure.
        
        Returns:
            Tuple of (is_valid, error_message)
        """
        if len(data) < 10:
            return False, "Data too short for JPG file"
        
        # Check JPG signature
        if not data.startswith(self.JPG_SOI):
            return False, f"Missing JPG SOI marker (found: {data[:2].hex()})"
        
        # Check for end marker
        if not data.endswith(self.JPG_EOI):
            return False, "Missing JPG EOI marker"
        
        # Look for essential JPG segments
        required_markers = [self.JPG_DQT, self.JPG_DHT, self.JPG_SOF0, self.JPG_SOS]
        found_markers = []
        
        i = 2  # Skip SOI
        while i < len(data) - 1:
            if data[i] == 0xFF and data[i + 1] != 0xFF:
                marker = data[i:i+2]
                if marker in required_markers:
                    found_markers.append(marker)
                
                # Skip to next marker
                if i + 2 < len(data):
                    try:
                        segment_length = struct.unpack('>H', data[i+2:i+4])[0]
                        i += 2 + segment_length
                    except:
                        i += 2
                else:
                    break
            else:
                i += 1
        
        missing_markers = [m for m in required_markers if m not in found_markers]
        if missing_markers:
            missing_names = []
            marker_names = {
                self.JPG_DQT: "DQT", self.JPG_DHT: "DHT", 
                self.JPG_SOF0: "SOF0", self.JPG_SOS: "SOS"
            }
            for marker in missing_markers:
                missing_names.append(marker_names.get(marker, marker.hex()))
            return False, f"Missing JPG markers: {', '.join(missing_names)}"
        
        return True, "Valid JPG structure"
    
    def try_repair_jpg(self, data: bytes) -> bytes:
        """
        Attempt to repair common JPG corruption issues.
        
        Returns:
            Repaired data
        """
        repaired = bytearray(data)
        
        # Ensure proper SOI marker
        if not repaired.startswith(self.JPG_SOI):
            self.logger.info("Adding missing SOI marker")
            repaired = bytearray(self.JPG_SOI) + repaired
        
        # Ensure proper EOI marker
        if not repaired.endswith(self.JPG_EOI):
            self.logger.info("Adding missing EOI marker")
            repaired += self.JPG_EOI
        
        # Remove any null bytes at the end (common corruption)
        while repaired and repaired[-1] == 0:
            repaired.pop()
            
        # Re-add EOI if we removed it
        if not repaired.endswith(self.JPG_EOI):
            repaired += self.JPG_EOI
        
        return bytes(repaired)
    
    def save_jpg(self, data: bytes, output_path: str) -> bool:
        """
        Save JPG data to file.
        
        Returns:
            True if successful, False otherwise
        """
        try:
            with open(output_path, 'wb') as f:
                f.write(data)
            
            self.logger.info(f"JPG saved to {output_path} ({len(data)} bytes)")
            return True
        
        except Exception as e:
            self.logger.error(f"Failed to save JPG: {e}")
            return False
    
    def analyze_data_chunks(self, ordered_data: List[Tuple[int, bytes]]) -> Dict:
        """Analyze the structure of data chunks from games."""
        analysis = {
            "total_chunks": len(ordered_data),
            "chunk_sizes": [],
            "size_distribution": {},
            "chunk_types": {},
            "missing_games": []
        }
        
        # Check for missing game IDs
        if ordered_data:
            min_id = ordered_data[0][0]
            max_id = ordered_data[-1][0]
            present_ids = set(game_id for game_id, _ in ordered_data)
            
            for i in range(min_id, max_id + 1):
                if i not in present_ids:
                    analysis["missing_games"].append(i)
        
        # Analyze chunk sizes
        for game_id, data in ordered_data:
            size = len(data)
            analysis["chunk_sizes"].append(size)
            
            size_range = f"{size//100*100}-{size//100*100+99}"
            analysis["size_distribution"][size_range] = analysis["size_distribution"].get(size_range, 0) + 1
            
            # Try to identify chunk type
            chunk_type = "unknown"
            if data.startswith(b'\xff\xd8'):
                chunk_type = "jpg_start"
            elif data.endswith(b'\xff\xd9'):
                chunk_type = "jpg_end"
            elif b'\xff' in data[:10]:
                chunk_type = "jpg_segment"
            elif data.startswith(b'JFIF'):
                chunk_type = "jfif_header"
            elif len(set(data)) < 10:
                chunk_type = "repeated_bytes"
            else:
                chunk_type = "binary_data"
            
            analysis["chunk_types"][chunk_type] = analysis["chunk_types"].get(chunk_type, 0) + 1
        
        return analysis
    
    def assemble_jpg(self, output_path: str = "assembled_image.jpg", 
                     force_repair: bool = True) -> bool:
        """
        Main assembly function.
        
        Args:
            output_path: Path for output JPG file
            force_repair: Whether to attempt repair if validation fails
            
        Returns:
            True if successful, False otherwise
        """
        self.logger.info("Starting JPG assembly process")
        
        # Order and extract game data
        ordered_data = self.order_game_data()
        if not ordered_data:
            self.logger.error("No binary data extracted from game outputs")
            return False
        
        # Analyze data structure
        analysis = self.analyze_data_chunks(ordered_data)
        self.logger.info(f"Data analysis: {analysis['total_chunks']} chunks")
        
        if analysis["missing_games"]:
            self.logger.warning(f"Missing data from games: {analysis['missing_games'][:10]}...")
        
        # Concatenate all data
        jpg_data = self.concatenate_data(ordered_data)
        
        # Validate JPG structure
        is_valid, validation_msg = self.validate_jpg_structure(jpg_data)
        self.logger.info(f"JPG validation: {validation_msg}")
        
        # Attempt repair if needed
        if not is_valid and force_repair:
            self.logger.info("Attempting to repair JPG data")
            jpg_data = self.try_repair_jpg(jpg_data)
            
            # Re-validate
            is_valid, validation_msg = self.validate_jpg_structure(jpg_data)
            self.logger.info(f"Post-repair validation: {validation_msg}")
        
        # Save the JPG file
        success = self.save_jpg(jpg_data, output_path)
        
        if success:
            # Save analysis data
            analysis_path = output_path.replace('.jpg', '_analysis.json')
            with open(analysis_path, 'w') as f:
                json.dump(analysis, f, indent=2)
            
            self.logger.info(f"Assembly complete! Image saved to {output_path}")
            if not is_valid:
                self.logger.warning("JPG may be corrupted - manual inspection recommended")
        
        return success
    
    def create_test_images(self, output_dir: str = "test_images"):
        """Create test images from subsets of data for debugging."""
        os.makedirs(output_dir, exist_ok=True)
        
        ordered_data = self.order_game_data()
        if not ordered_data:
            return
        
        # Try different chunk combinations
        test_configs = [
            ("first_100", ordered_data[:100]),
            ("last_100", ordered_data[-100:]),
            ("middle_100", ordered_data[len(ordered_data)//2-50:len(ordered_data)//2+50]),
            ("every_10th", ordered_data[::10]),
        ]
        
        for name, data_subset in test_configs:
            if not data_subset:
                continue
            
            test_data = self.concatenate_data(data_subset)
            test_path = os.path.join(output_dir, f"test_{name}.jpg")
            
            # Try repair
            test_data = self.try_repair_jpg(test_data)
            
            self.save_jpg(test_data, test_path)
            self.logger.info(f"Test image created: {test_path}")


def main():
    """Command line interface for JPG assembler."""
    if len(sys.argv) < 2:
        print("Usage: python assemble_jpg.py <results_file> [options]")
        print("Options:")
        print("  --output FILE    Output JPG file (default: assembled_image.jpg)")
        print("  --no-repair      Don't attempt to repair JPG structure")
        print("  --test-images    Create test images from data subsets")
        print("  --analyze-only   Only analyze data structure, don't create JPG")
        sys.exit(1)
    
    results_file = sys.argv[1]
    output_path = "assembled_image.jpg"
    force_repair = True
    create_tests = False
    analyze_only = False
    
    # Parse command line options
    i = 2
    while i < len(sys.argv):
        if sys.argv[i] == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == "--no-repair":
            force_repair = False
            i += 1
        elif sys.argv[i] == "--test-images":
            create_tests = True
            i += 1
        elif sys.argv[i] == "--analyze-only":
            analyze_only = True
            i += 1
        else:
            print(f"Unknown option: {sys.argv[i]}")
            sys.exit(1)
    
    try:
        assembler = JPGAssembler(results_file)
        
        if analyze_only:
            # Just analyze the data structure
            ordered_data = assembler.order_game_data()
            analysis = assembler.analyze_data_chunks(ordered_data)
            
            print("=== DATA ANALYSIS ===")
            print(f"Total chunks: {analysis['total_chunks']}")
            print(f"Missing games: {len(analysis['missing_games'])}")
            if analysis['missing_games'][:10]:
                print(f"First missing: {analysis['missing_games'][:10]}")
            
            print(f"\nChunk types:")
            for chunk_type, count in analysis['chunk_types'].items():
                print(f"  {chunk_type}: {count}")
            
            print(f"\nSize distribution:")
            for size_range, count in analysis['size_distribution'].items():
                print(f"  {size_range} bytes: {count}")
        
        else:
            # Assemble the JPG
            success = assembler.assemble_jpg(output_path, force_repair)
            
            if create_tests:
                assembler.create_test_images()
            
            if success:
                print(f"JPG assembly complete: {output_path}")
            else:
                print("JPG assembly failed")
                sys.exit(1)
    
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()