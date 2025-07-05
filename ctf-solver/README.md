# CTF Auto-Rev Challenge Solver

A comprehensive toolkit for solving CTF reverse engineering challenges with 11,424 games and assembling the final JPG image.

## Overview

This toolkit provides a complete solution for a CTF challenge that involves:
- Solving 11,424 individual games
- Each game requires 8 bytes of input
- Games use dynamic .so file loading and complex transformations
- Successful games output data that must be assembled into a JPG image

## Components

### 1. `solve_single.py` - Single Game Solver
Solves individual CTF games using multiple strategies:
- **RC4-like transformation simulation** based on analysis
- **Brute force search** with intelligent patterns
- **Direct game execution** for verification
- **Target validation** against known values (0x3d2c5b74, 0xf060203ec)

**Usage:**
```bash
python solve_single.py <game_executable> [so_files...]
python solve_single.py ./game1 lib1.so lib2.so
```

### 2. `analyze_games.py` - Pattern Analysis Tool
Analyzes patterns across multiple games to optimize solving:
- **SO file analysis** - symbols, dependencies, hashes
- **Pattern recognition** - identical files, common symbols
- **Game clustering** - group similar games
- **Strategy suggestions** - optimization recommendations

**Usage:**
```bash
python analyze_games.py <games_directory> [output_file]
python analyze_games.py ./ctf_games analysis.json
```

### 3. `mass_solver.py` - Automated Mass Solver
Solves all 11,424 games using parallel processing:
- **Parallel execution** - configurable workers (threads/processes)
- **Progress tracking** - real-time statistics
- **Error handling** - timeouts and graceful failure handling
- **Result persistence** - save/load solving progress

**Usage:**
```bash
python mass_solver.py <games_directory> [options]
python mass_solver.py ./ctf_games --workers 16 --timeout 60
```

**Options:**
- `--workers N` - Number of parallel workers (default: 8)
- `--timeout N` - Timeout per game in seconds (default: 30)
- `--processes` - Use processes instead of threads
- `--load FILE` - Load previous results instead of solving
- `--output FILE` - Output file for results

### 4. `assemble_jpg.py` - JPG Assembly Tool
Assembles the final JPG image from successful game outputs:
- **Data extraction** - hex, base64, binary from game outputs
- **Proper ordering** - sort by game ID for correct sequence
- **JPG validation** - verify image structure and markers
- **Auto-repair** - fix common corruption issues

**Usage:**
```bash
python assemble_jpg.py <results_file> [options]
python assemble_jpg.py mass_solve_results.json --output final_image.jpg
```

**Options:**
- `--output FILE` - Output JPG file name
- `--no-repair` - Don't attempt to repair JPG structure
- `--test-images` - Create test images from data subsets
- `--analyze-only` - Only analyze data structure

## Complete Workflow

### Step 1: Analyze Games (Optional but Recommended)
```bash
# Analyze all games to understand patterns
python analyze_games.py ./ctf_games game_analysis.json

# Review analysis results
cat game_analysis.json | jq '.solving_suggestions'
```

### Step 2: Test Single Game Solving
```bash
# Test the solver on a single game first
python solve_single.py ./ctf_games/game1/game
```

### Step 3: Mass Solve All Games
```bash
# Solve all games with parallel processing
python mass_solver.py ./ctf_games --workers 16 --timeout 60 --output results.json

# Monitor progress in real-time
tail -f mass_solver.log
```

### Step 4: Assemble Final JPG
```bash
# Assemble the JPG from successful game outputs
python assemble_jpg.py results.json --output ctf_solution.jpg

# Create test images for debugging if needed
python assemble_jpg.py results.json --test-images
```

## Technical Details

### Game Challenge Structure
Each game in the CTF challenge:
- Expects exactly 8 bytes of input via stdin
- Loads multiple .so files dynamically based on hardcoded names
- Applies RC4-like S-box initialization and permutation
- Performs complex transformations using loaded functions
- Checks results against target values: `0x3d2c5b74` and `0xf060203ec`
- Outputs "You win!" plus additional binary data on success

### Solving Strategies

**1. Mathematical Approach:**
- Simulate the RC4-like transformation algorithm
- Test input combinations against known target values
- Fast validation without running actual game executable

**2. Brute Force with Patterns:**
- Try common patterns first (zeros, sequential, ASCII)
- Use target values as hints for input construction
- Systematic search through byte combinations

**3. Game Analysis Integration:**
- Group games with identical .so files (same solution)
- Prioritize games with common symbols/patterns
- Skip redundant solving for clustered games

### Error Handling
- **Timeouts:** Games that don't respond within timeout limit
- **Crashes:** Handle game executable crashes gracefully
- **Missing files:** Skip games with missing executables or .so files
- **Partial results:** Save progress periodically for recovery

### Performance Optimization
- **Parallel processing:** Multiple games solved simultaneously
- **Smart caching:** Reuse solutions for identical games
- **Progressive solving:** Start with easier patterns
- **Memory management:** Efficient handling of large datasets

## File Structure
```
ctf-solver/
├── solve_single.py      # Single game solver
├── analyze_games.py     # Pattern analysis tool
├── mass_solver.py       # Mass automation solver
├── assemble_jpg.py      # JPG assembly tool
└── README.md           # This documentation
```

## Requirements
- Python 3.7+
- Standard libraries: `subprocess`, `threading`, `concurrent.futures`
- System tools: `nm`, `ldd`, `strings` (for .so analysis)
- CTF challenge files in expected directory structure

## Expected Directory Structure
```
ctf_games/
├── game1/
│   ├── game            # Executable
│   ├── lib1.so         # Dynamic library
│   └── lib2.so         # Dynamic library
├── game2/
│   ├── game            # Executable
│   └── *.so files
...
├── game11424/
│   ├── game            # Executable
│   └── *.so files
```

## Output Files
- `game_analysis.json` - Pattern analysis results
- `mass_solve_results.json` - Solving results and successful outputs
- `mass_solver.log` - Detailed solving log
- `assembled_image.jpg` - Final CTF solution image
- `test_images/` - Debugging test images (optional)

## Troubleshooting

### No Solutions Found
- Check game executable permissions and dependencies
- Verify .so files are accessible and compatible
- Try reducing timeout or increasing brute force attempts
- Review game analysis for patterns

### JPG Assembly Issues
- Use `--test-images` to create partial images for debugging
- Check `*_analysis.json` for data structure insights
- Try `--no-repair` if auto-repair causes issues
- Manually inspect game outputs for binary data format

### Performance Issues
- Reduce number of workers if system is overwhelmed
- Use processes instead of threads for CPU-intensive tasks
- Implement result caching for repeated game patterns
- Monitor memory usage with large datasets

## Contributing
This toolkit is designed to be modular and extensible. Key areas for enhancement:
- Additional transformation algorithms
- More sophisticated pattern recognition
- Alternative assembly strategies
- Performance optimizations

## License
MIT License - See individual files for specific license information.