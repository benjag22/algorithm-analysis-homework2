/**
 * Copyright @leonardlover on GitHub
 * https://github.com/leonardlover/uhr
 * Modified for this project
 */

#pragma once

#include <chrono>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "uhr_utils.hpp"

/**
 * Run performance measurements.
 *
 * @tparam ED Derivate class of EditDistance to use.
 * @param filePath Path of the file where performance data will be written.
 * @param runs Number of runs per test case. Should be >= 32.
 * @param lower Range of test cases. Should be > 0.
 * @param upper Range of test cases. Should be > 0.
 * @param step Step of test cases. Should be > 0.
 * @param text_extractor1 Text extractor for the first file.
 * @param text_extractor2 Text extractor for the second file.
 */
template <typename ED, std::enable_if_t<std::is_base_of_v<EditDistance, ED>>* = nullptr>
void uhr(
    const std::string &filePath,
    const uint64_t runs,
    const uint64_t lower,
    const uint64_t upper,
    const uint64_t step,
    TextExtractor &text_extractor1,
    TextExtractor &text_extractor2
) {
    validate_input(runs, lower, upper, step);

    const uint64_t total_runs_additive = runs * ((upper - lower) / step + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    std::chrono::duration<double, std::nano> elapsed_time{};

    // File to write time data
    std::ofstream time_data(filePath);
    time_data << "n,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4\n";

    // Begin testing
    const std::string test_name = std::filesystem::path(filePath).stem().string();
    std::cout << "Running " << test_name << " tests...\n\n";
    uint64_t executed_runs = 0;

    for (uint64_t n = lower; n <= upper; n += step) {
        double mean_time = 0;
        double time_stdev = 0;

        // Test configuration goes here
        const std::string &string1 = text_extractor1.extract(n);
        const std::string &string2 = text_extractor2.extract(n);

        // Run to compute elapsed time
        for (size_t i = 0; i < runs; i++) {
            // Remember to change total depending on step type
            display_progress(++executed_runs, total_runs_additive);

            ED edit_distance(string1, string2);
            auto begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            edit_distance.calculate_distance();
            auto end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();

            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (size_t i = 0; i < runs; i++) {
            const double dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1; // Subtract 1 to get unbiased estimator
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        time_data << n << ',' << mean_time << ',' << time_stdev << ',';
        time_data << q[0] << ',' << q[1] << ',' << q[2] << ',' << q[3] << ',' << q[4] << '\n';
    }

    // This is to keep loading bar after testing
    std::cout << "\n\n" << test_name << " done!" << std::endl;

    time_data.close();
}
