#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "cache.hpp"

// Usage:
//   ./cache_sim <trace_file> [numSets] [associativity] [blockSizeBytes]
//
// Trace file format: one hex address per line, e.g.
//   0x00400000
//   0x00400004
//   0x10010000
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <trace_file> [numSets=64] [associativity=4] [blockSizeBytes=64]\n";
        return 1;
    }

    std::string tracePath = argv[1];
    uint32_t numSets = (argc > 2) ? std::stoul(argv[2]) : 64;
    uint32_t associativity = (argc > 3) ? std::stoul(argv[3]) : 4;
    uint32_t blockSize = (argc > 4) ? std::stoul(argv[4]) : 64;

    std::ifstream trace(tracePath);
    if (!trace.is_open()) {
        std::cerr << "Failed to open trace file: " << tracePath << "\n";
        return 1;
    }

    Cache cache(numSets, associativity, blockSize);

    std::string line;
    while (std::getline(trace, line)) {
        if (line.empty()) continue;
        uint64_t address = std::stoull(line, nullptr, 16);
        cache.access(address);
    }

    std::ostringstream label;
    label << numSets << " sets, " << associativity << "-way, "
          << blockSize << "B blocks";
    cache.stats().print(label.str());

    return 0;
}
