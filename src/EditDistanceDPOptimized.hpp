#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "EditDistance.hpp"

class EditDistanceDPOptimized final : public EditDistance {
    std::vector<int> m_prev;
    std::vector<int> m_curr;

public:
    EditDistanceDPOptimized(const std::string &s1, const std::string &s2)
        : EditDistance(s1, s2),
          m_prev(s2.length() + 1, 0),
          m_curr(s2.length() + 1, 0) {}

    int calculate_distance() override {
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] uint64_t calculate_memory() const override {
        return m_prev.size() * sizeof(int) + m_curr.size() * sizeof(int);
    }

private:
    int calculate(const int m, const int n) override {
        for (int j = 0; j <= n; ++j) {
            m_prev[j] = j;
        }

        for (int i = 1; i <= m; ++i) {
            m_curr[0] = i;

            for (int j = 1; j <= n; ++j) {
                if (m_s1[i - 1] == m_s2[j - 1]) {
                    m_curr[j] = m_prev[j - 1];
                } else {
                    m_curr[j] = 1 + std::min(Insert(i, j), Delete(i, j));
                }
            }

            std::swap(m_prev, m_curr);
        }

        return m_prev[n];
    }

    int Insert(const int m, const int n) override {
        return m_curr[n - 1];
    }

    int Delete(const int m, const int n) override {
        return m_prev[n];
    }
};
