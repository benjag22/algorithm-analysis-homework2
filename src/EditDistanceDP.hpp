#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "EditDistance.hpp"

class EditDistanceDP final : EditDistance {
    mutable std::vector<std::vector<int>> m_dp;

public:
    EditDistanceDP(const std::string &s1, const std::string &s2): EditDistance(s1, s2) {
        m_dp.resize(s1.length() + 1, std::vector(s2.length() + 1, -1));
    }

    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        if (m_dp[m][n] != -1) {
            return m_dp[m][n];
        }

        if (m == 0) {
            return m_dp[m][n] = n;
        }
        if (n == 0) {
            return m_dp[m][n] = m;
        }

        if (m_s1[m - 1] == m_s2[n - 1]) {
            return m_dp[m][n] = calculate(m - 1, n - 1);
        }

        const int insert_cost = Insert(m, n);
        const int delete_cost = Delete(m, n);

        return m_dp[m][n] = 1 + std::min(insert_cost, delete_cost);
    }

    int Insert(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m, n - 1);
    }

    int Delete(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m - 1, n);
    }

    int calculate_iterative() const {
        const int m = m_s1.length();
        const int n = m_s2.length();

        std::vector table(m + 1, std::vector(n + 1, 0));

        for (int i = 0; i <= m; ++i) {
            for (int j = 0; j <= n; ++j) {
                if (i == 0) {
                    table[i][j] = j;
                } else if (j == 0) {
                    table[i][j] = i;
                } else if (m_s1[i - 1] == m_s2[j - 1]) {
                    table[i][j] = table[i - 1][j - 1];
                } else {
                    table[i][j] = 1 + std::min(table[i - 1][j], table[i][j - 1]);
                }
            }
        }

        return table[m][n];
    }

    int calculate_distance() override {
        return calculate_iterative();
    }

    int get_dp_size() const {
        return m_dp.size() * (m_dp.empty() ? 0 : m_dp[0].size());
    }
};
