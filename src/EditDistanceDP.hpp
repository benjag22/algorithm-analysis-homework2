#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "EditDistance.hpp"

class EditDistanceDP final : EditDistance {
    std::vector<std::vector<int>> m_table;

public:
    EditDistanceDP(const std::string &s1, const std::string &s2)
        : EditDistance(s1, s2),
          m_table(s1.length() + 1, std::vector(s2.length() + 1, 0)) {}

    int calculate_distance() override {
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] uint64_t calculate_memory() const override {
        uint64_t memory = 0;

        for (const auto &row : m_table) {
            memory += row.size() * sizeof(row[0]);
        }

        return memory;
    }

private:
    int calculate(const int m, const int n) override {
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0) {
                    m_table[i][j] = j;
                } else if (j == 0) {
                    m_table[i][j] = i;
                } else if (m_s1[i - 1] == m_s2[j - 1]) {
                    m_table[i][j] = m_table[i - 1][j - 1];
                } else {
                    m_table[i][j] = 1 + std::min(Insert(i, j), Delete(i, j));
                }
            }
        }

        return m_table[m][n];
    }

    int Insert(const int m, const int n) override {
        return m_table[m][n - 1];
    }

    int Delete(const int m, const int n) override {
        return m_table[m - 1][n];
    }
};
