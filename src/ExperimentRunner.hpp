#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "EditDistance.hpp"
#include "EditDistanceDP.hpp"
#include "EditDistanceDPOptimized.hpp"
#include "EditDistanceMemo.hpp"
#include "PerformanceMeter.hpp"
#include "TextExtractor.hpp"

class ExperimentRunner {
    std::vector<std::string> m_texts;
    // algorithm -> source text -> destination text
    std::vector<std::vector<std::vector<PerformanceMeter::Result>>> m_results;

public:
    ExperimentRunner(
        const std::string &file_path,
        const int start_pos,
        const int extract_length,
        const int number_extracts
    ) : m_texts(TextExtractor(file_path).extract_multiple_texts(start_pos, extract_length, number_extracts)),
        m_results(4, std::vector(number_extracts, std::vector<PerformanceMeter::Result>(number_extracts))) {}

    void run_all_tests() {
        // run_tests<EditDistanceRecursive>("Recursive", 0);
        run_tests<EditDistanceMemo>("Recursive Memoized", 1);
        run_tests<EditDistanceDP>("Dynamic Programming", 2);
        run_tests<EditDistanceDPOptimized>("Dynamic Programming Optimized", 3);
    }

private:
    template <typename T, std::enable_if_t<std::is_base_of_v<EditDistance, T>>* = nullptr>
    void run_tests(const char *name, const int algo_idx) {
        const int n = m_texts.size();
        std::cout << "\n=== Running " << n * (n - 1) << " tests for " << name << " ===" << std::endl;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    run_test<T>(algo_idx, i, j);
                }
            }
        }
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<EditDistance, T>>* = nullptr>
    void run_test(const int algo_idx, const int source_idx, const int target_idx) {
        const std::string &source = m_texts[source_idx];
        const std::string &target = m_texts[target_idx];

        T algorithm(source, target);
        PerformanceMeter::Result &result = m_results[algo_idx][source_idx][target_idx];
        result = PerformanceMeter::measure(algorithm);

        const double megabytes = result.memory_bytes / 1e6;

        std::cout << "Distance = " << result.distance << " \t"
                << "Time = " << result.time_ms << "ms \t\t"
                << "Memory (approx.) -> " << megabytes << " MB" << std::endl;
    }
};
