#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "edit_distance.hpp"

class EditDistanceMemo final : public EditDistance {
    uint64_t m_call_count = 0;
    std::vector<std::vector<int>> m_table;

public:
    EditDistanceMemo(const std::string &s1, const std::string &s2)
        : EditDistance(s1, s2),
          m_table(s1.length() + 1, std::vector(s2.length() + 1, -1)) {}

    int calculate_distance() override {
        m_call_count = 0;
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] uint64_t calculate_memory() const override {
        uint64_t memory = m_call_count * sizeof(void *);

        for (const auto &row : m_table) {
            memory += row.size() * sizeof(row[0]);
        }

        return memory;
    }

private:
    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        m_call_count++;

        int &value = m_table[m][n];

        if (value != -1) {
            return value;
        }

        if (m == 0) {
            return value = n;
        }

        if (n == 0) {
            return value = m;
        }

        if (m_s1[m - 1] == m_s2[n - 1]) {
            return value = calculate(m - 1, n - 1);
        }

        return value = 1 + std::min(Insert(m, n), Delete(m, n));
    }

    int Insert(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m, n - 1);
    }

    int Delete(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m - 1, n);
    }
};
