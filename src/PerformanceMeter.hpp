#pragma once

#include <chrono>
#include <functional>

#include "EditDistance.hpp"

class PerformanceMeter {
public:
    struct Result {
        int distance;
        long long time_ms;
        size_t memory_bytes;
    };

    template <typename T, std::enable_if_t<std::is_base_of_v<EditDistance, T>>* = nullptr>
    static Result measure(T &algorithm, const std::function<size_t(const T &)> &memory_fn) {
        namespace chrono = std::chrono;

        Result result{};

        const auto start = chrono::high_resolution_clock::now();
        result.distance = algorithm.calculate_distance();
        const auto end = chrono::high_resolution_clock::now();

        result.time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        result.memory_bytes = memory_fn(algorithm);

        return result;
    }
};
