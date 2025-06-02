#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <functional>
#include "EditDistance.h"
using namespace std;

class PerformanceMeter {
public:
    struct Result{
        int distance;
        long long timeMs;
        size_t memoryBytes;
    };
    template<typename T>
    static Result measure(T& algorithm, function<size_t(const T&)> memoryFunc) {
        Result result;

        auto start = chrono::high_resolution_clock::now();
        result.distance = algorithm.CalculateDistante();
        auto end = chrono::high_resolution_clock::now();

        result.timeMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        result.memoryBytes = memoryFunc(algorithm);

        return result;
    }
};