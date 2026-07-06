#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>

// A single cache line (block) within a set.
struct CacheLine {
    bool valid = false;
    uint64_t tag = 0;
};

// Tracks hit/miss counts and derived stats for a simulation run.
struct CacheStats {
    uint64_t accesses = 0;
    uint64_t hits = 0;
    uint64_t misses = 0;

    double hitRate() const {
        return accesses == 0 ? 0.0 : static_cast<double>(hits) / accesses;
    }

    void print(const std::string& label) const;
};

// Configurable N-way set-associative cache with LRU replacement.
//
// Address breakdown (for a 64-bit address):
//   [ tag | set index | block offset ]
// - blockSize determines how many low bits are the block offset
// - numSets determines how many bits (above the offset) are the set index
// - whatever remains is the tag
class Cache {
public:
    Cache(uint32_t numSets, uint32_t associativity, uint32_t blockSizeBytes);

    // Simulates one memory access. Returns true on hit, false on miss.
    bool access(uint64_t address);

    const CacheStats& stats() const { return stats_; }

private:
    uint32_t numSets_;
    uint32_t associativity_;
    uint32_t blockSizeBytes_;

    uint32_t offsetBits_;
    uint32_t indexBits_;

    // One vector of ways per set. Each way holds a CacheLine.
    std::vector<std::vector<CacheLine>> sets_;

    // Per-set LRU order: front = most recently used, back = least recently used.
    std::vector<std::list<uint32_t>> lruOrder_;

    CacheStats stats_;

    // Splits an address into its tag and set index.
    void decodeAddress(uint64_t address, uint64_t& tag, uint32_t& setIndex) const;
};
