
# Advent of Code 2023

| Day | Part 1 | Part 2 |
|-----|--------|--------|
| 1   | \*     | \*     | 
| 2   | \*     | \*     | 
| 3   | \*     | \*     | 
| 4   | \*     | \*     | 
| 5   | \*     | \*     | 
| 6   | \*     | \*     | 
| 7   | \*     | \*     | 
| 8   | \*     | ...    | 
| 9   | \*     | \*     | 
| 10  | \*     | \*     | 

## Build

### Setup

#### 1. Clone the repository

```bash
git clone git@github.com:NathanPERIER/AdventOfCode2023.git
cd AdventOfCode2023
```

#### 2. Create the build directories

```bash
meson setup build_debug --buildtype=debug
meson setup build_optim --buildtype=debugoptimised
meson setup build_release --buildtype=release
```

### Compilation

```bash
cd build_debug
ninja
```

### Run

```bash
cat input-1.txt | src/day-1
```

