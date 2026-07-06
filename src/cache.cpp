#include "cache.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

void CacheStats::print(const std::string& label) const {
    std::cout << "--- " << label << " ---\n"
              << "Accesses:  " << accesses << "\n"
              << "Hits:      " << hits << "\n"
              << "Misses:    " << misses << "\n"
              << "Hit rate:  " << (hitRate() * 100.0) << "%\n";
}

namespace {
// log2 helper for power-of-two sizes; throws if not a power of two.
uint32_t log2Exact(uint32_t n) {
    if (n == 0 || (n & (n - 1)) != 0) {
        throw std::invalid_argument("value must be a power of two");
    }
    uint32_t bits = 0;
    while ((1u << bits) < n) ++bits;
    return bits;
}
} // namespace

Cache::Cache(uint32_t numSets, uint32_t associativity, uint32_t blockSizeBytes)
    : numSets_(numSets),
      associativity_(associativity),
      blockSizeBytes_(blockSizeBytes),
      offsetBits_(log2Exact(blockSizeBytes)),
      indexBits_(log2Exact(numSets)),
      sets_(numSets, std::vector<CacheLine>(associativity)),
      lruOrder_(numSets) {
    // Initialize each set's LRU order with way indices 0..associativity-1.
    for (auto& order : lruOrder_) {
        for (uint32_t way = 0; way < associativity_; ++way) {
            order.push_back(way);
        }
    }
}

void Cache::decodeAddress(uint64_t address, uint64_t& tag, uint32_t& setIndex) const {
    uint64_t blockAddr = address >> offsetBits_;
    setIndex = static_cast<uint32_t>(blockAddr & ((1ull << indexBits_) - 1));
    tag = blockAddr >> indexBits_;
}

bool Cache::access(uint64_t address) {
    uint64_t tag;
    uint32_t setIndex;
    decodeAddress(address, tag, setIndex);
    stats_.accesses++;

    auto& ways = sets_[setIndex];
    auto& order = lruOrder_[setIndex];

    for (uint32_t way = 0; way < associativity_; ++way) {
        if (ways[way].valid && ways[way].tag == tag) {
            stats_.hits++;
            order.remove(way);
            order.push_front(way);
            return true;
        }
    }

    stats_.misses++;
    uint32_t victim = order.back();
    order.pop_back();
    ways[victim].valid = true;
    ways[victim].tag = tag;
    order.push_front(victim);
    return false;
}
