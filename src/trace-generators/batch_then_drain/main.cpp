//
// Created by Ali Kooshesh on 11/1/25.
//

#include <iostream>
#include <fstream>
#include <random>
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <filesystem>   // ⬅️ add this

#include "../../../utils/TraceConfig.hpp"

void generateTrace(const unsigned seed,
                   const std::size_t n,
                   TraceConfig &config,
                   std::uniform_int_distribution<int> &dist,
                   std::mt19937& gen ) {

    // create and open the output file name
    std::string outputFileName = config.makeTraceFileName(seed, n);

    // If TraceConfig gives us a path like "../../../traces/...",
    // strip the leading "../../../" so it becomes "traces/..."
    const std::string prefix = "../../../";
    if (outputFileName.rfind(prefix, 0) == 0) { // if it starts with prefix
        outputFileName = outputFileName.substr(prefix.size());
    }

    std::cout << "File name: " << outputFileName << std::endl;

    // Ensure the directory exists (e.g., traces/batch_then_drain)
    std::filesystem::path outPath(outputFileName);
    std::filesystem::create_directories(outPath.parent_path());

    std::ofstream out(outputFileName.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to open file " << outputFileName << std::endl;
        std::exit(1);
    }

    out << config.profileName << " " << n << " " << seed << std::endl;

    // ... rest of your insert/extract code ...
    unsigned id = 0;                 // unique ID for tie-breaking
    const int spaceBeforeNumber = 10;

    // 1) Batch phase: N inserts
    for (std::size_t i = 0; i < n; ++i) {
        int key = dist(gen);
        out << "I "
            << std::setw(spaceBeforeNumber) << key
            << std::setw(spaceBeforeNumber) << id++
            << "\n";
    }

    // 2) Drain phase: N extracts
    for (std::size_t i = 0; i < n; ++i) {
        out << "E\n";
    }

    out.close();
}

int main() {

    // TraceConfig provides pre-configured values such as Ns and seeds.
    // IMPORTANT: profileName should be "batch_then_drain"
    TraceConfig config(std::string("batch_then_drain"));

    for (auto seed : config.seeds) {        // currently, we are using one seed only.
        std::mt19937 rng(seed);             // random-number generator using "seed"

        for (auto n : config.Ns) {

            // For batch-then-drain, we do NOT want many duplicates.
            // Use a wide range of keys, e.g. [1, n] or even [1, 10*n].
            const unsigned key_min = 1;
            const unsigned key_max = static_cast<unsigned>(n);   // wide enough to avoid lots of duplicates

            std::uniform_int_distribution<int> dist(key_min, key_max);

            generateTrace(seed, n, config, dist, rng);
        }
    }

    return 0;
    
}
