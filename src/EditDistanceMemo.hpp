#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "EditDistance.hpp"

class EditDistanceMemo final : public EditDistance {
    int m_call_count = 0;
    std::vector<std::vector<int>> m_memory;

public:
    EditDistanceMemo(const std::string &s1, const std::string &s2) : EditDistance(s1, s2) {
        m_memory = std::vector(s1.length() + 1, std::vector(s2.length() + 1, -1));
    }

    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        m_call_count++;

        if (m_memory[m][n] != -1) {
            return m_memory[m][n];
        }

        if (m == 0)
            return m_memory[m][n] = n;

        if (n == 0)
            return m_memory[m][n] = m;

        if (m_s1[m - 1] == m_s2[n - 1]) {
            return m_memory[m][n] = calculate(m - 1, n - 1);
        }

        return m_memory[m][n] = 1 + std::min(calculate(m, n - 1), calculate(m - 1, n));
    }

    int Insert(const int m, const int n) override {
        return calculate(m, n - 1);
    }

    int Delete(const int m, const int n) override {
        return calculate(m - 1, n);
    }

    int calculate_distance() override {
        m_call_count = 0;
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] int get_call_count() const {
        return m_call_count;
    }
};
