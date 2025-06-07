#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "EditDistance.hpp"

class EditDistanceDPOptimized final : public EditDistance {
public:
    EditDistanceDPOptimized(const std::string &s1, const std::string &s2) : EditDistance(s1, s2) {}

    [[nodiscard]] int calculate_iterative() const {
        const int m = m_s1.length();
        const int n = m_s2.length();

        std::vector prev(n + 1, 0);
        std::vector curr(n + 1, 0);

        for (int j = 0; j <= n; ++j) {
            prev[j] = j;
        }

        for (int i = 1; i <= m; ++i) {
            curr[0] = i;

            for (int j = 1; j <= n; ++j) {
                if (m_s1[i - 1] == m_s2[j - 1]) {
                    curr[j] = prev[j - 1];
                } else {
                    curr[j] = 1 + std::min(curr[j - 1], prev[j]);
                }
            }

            std::swap(prev, curr);
        }

        return prev[n];
    }

    int Insert(int m, int n) override {
        return -1;
    }

    int Delete(int m, int n) override {
        return -1;
    }

    int calculate(int m, int n) override {
        return calculate_iterative();
    }

    int calculate_distance() override {
        return calculate_iterative();
    }
};
