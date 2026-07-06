# Cache Simulator

A configurable N-way set-associative cache simulator in C++, built to explore
cache design tradeoffs (associativity, block size, replacement policy) using
synthetic memory access traces.

Built as a hands-on refresher on computer architecture and C++.

## Status

✅ Core simulator working. Address decoding, hit/miss detection, and LRU
eviction are implemented in `src/cache.cpp` / `src/cache.hpp`.

## Design

- Configurable number of sets, associativity (ways per set), and block size
- LRU replacement policy (first pass)
- Address is split into `[ tag | set index | block offset ]`
- Reads a trace of hex memory addresses (one per line) and reports hit/miss
  statistics

## Build

Requires CMake 3.10+ and a C++17 compiler.

```bash
mkdir build && cd build
cmake ..
make
```

## Run

```bash
./cache_sim ../traces/sample.trace [numSets] [associativity] [blockSizeBytes]

# Example: 64 sets, 4-way associative, 64-byte blocks (defaults)
./cache_sim ../traces/sample.trace 64 4 64
```

## Generate a trace

`scripts/generate_trace.py` produces a synthetic trace with a mix of a hot
working set, sequential access, and random noise — enough variation to make
associativity and replacement policy actually matter.

```bash
python3 scripts/generate_trace.py --out traces/sample.trace --n 5000
```

## Roadmap

- [ ] Verify hit rate against hand-computed expectations on a tiny trace
- [ ] Add a second replacement policy (FIFO or random) for comparison
- [ ] Sweep associativity / set count and plot hit rate vs. configuration
- [ ] (Stretch) Support multi-level cache (L1 + L2)

## Why this project

Written to build practical C/C++ fluency and reinforce computer-architecture
fundamentals: cache hierarchies, replacement policies, and address decoding.
