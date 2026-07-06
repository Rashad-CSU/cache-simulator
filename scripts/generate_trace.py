#!/usr/bin/env python3
"""
Generate a synthetic memory-access trace for the cache simulator.

Produces a mix of:
  - sequential access (good locality — should hit a lot once warmed up)
  - a "hot" working set accessed repeatedly (tests LRU behavior)
  - some random noise addresses (tests eviction under pressure)

Usage:
    python3 generate_trace.py --out traces/sample.trace --n 5000
"""
import argparse
import random


def generate(n: int, seed: int) -> list[str]:
    rng = random.Random(seed)
    addresses = []

    base = 0x1000
    working_set = [base + i * 64 for i in range(32)]  # 32 blocks, hot set

    for _ in range(n):
        r = rng.random()
        if r < 0.6:
            # Access something from the hot working set (locality)
            addr = rng.choice(working_set)
        elif r < 0.85:
            # Sequential-ish scan
            addr = base + rng.randint(0, 200) * 64
        else:
            # Random noise across a wider address range
            addr = rng.randint(0, 0xFFFFF) & ~0x3F  # 64B aligned

        addresses.append(f"0x{addr:08x}")

    return addresses


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", default="traces/sample.trace")
    parser.add_argument("--n", type=int, default=5000)
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()

    addresses = generate(args.n, args.seed)
    with open(args.out, "w") as f:
        f.write("\n".join(addresses) + "\n")

    print(f"Wrote {len(addresses)} addresses to {args.out}")


if __name__ == "__main__":
    main()
