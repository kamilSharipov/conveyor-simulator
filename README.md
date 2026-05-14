# Conveyor Simulator

Simulation of a manufacturing shop with sequential processing of items and dynamic machine selection.

## Requirements

- C++17 compatible compiler
- CMake 3.14 or higher

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Build type can be Debug or Release. Debug adds -g -O0, Release adds -O2 -DNDEBUG -march=native. Warnings are enabled (-Wall -Wextra -Wpedantic), and in Release mode -Werror is added.

## Run

```bash
./build/simulator <input_file>
```

The program reads the input file, runs the simulation, and prints events to stdout. If the input format is invalid, the first erroneous line is printed and the program exits with code 1.

## Tests

Tests use Google Test. To build and run tests:

``` bash
cmake -B build -DBUILD_TESTING=ON
cmake --build build
cd build && ctest
```

## Input format

The input file must follow the specification:

First line: $M$ (number of item types) and $N$ (number of machines)

Next $M - 1$ lines: matrix of operation times $(T_{i,j})$

Then $N$ lines: each starts with the queue size $q_j$, followed by $q_j$ integers indicating the types of items in the initial queue

All numbers are integers within specified ranges.

## Output

Events are printed in chronological order, with a fixed priority on identical timestamps: 
- `finish`,
- `start`,
- `wait`,
- `ready`.