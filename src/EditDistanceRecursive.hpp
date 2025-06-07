#pragma once

#include <algorithm>
#include <string>

#include "EditDistance.hpp"

class EditDistanceRecursive final : EditDistance {
    int m_call_count = 0;

public:
    EditDistanceRecursive(const std::string &s1, const std::string &s2) : EditDistance(s1, s2) {}

    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        m_call_count++;

        if (m == 0) {
            return n;
        }

        if (n == 0) {
            return m;
        }

        if (m_s1[m - 1] == m_s2[n - 1]) {
            return calculate(m - 1, n - 1);
        }

        const int insert_cost = Insert(m, n);
        const int delete_cost = Delete(m, n);

        return 1 + std::min(insert_cost, delete_cost);
    }

    int Insert(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m, n - 1);
    }

    int Delete(const int m, const int n) override { // NOLINT(*-no-recursion)
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
