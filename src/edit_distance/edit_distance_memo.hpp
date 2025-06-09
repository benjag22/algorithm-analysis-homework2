#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "edit_distance.hpp"

class EditDistanceMemo final : public EditDistance {
    uint64_t m_call_count = 0;
    uint64_t m_current_depth = 0;
    uint64_t m_max_depth = 0;
    std::vector<std::vector<int>> m_table;

public:
    EditDistanceMemo(const std::string &s1, const std::string &s2)
        : EditDistance(s1, s2),
          m_table(s1.length() + 1, std::vector(s2.length() + 1, -1)) {}

    int calculate_distance() override {
        m_call_count = 0;
        m_current_depth = 0;
        m_max_depth = 0;
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] uint64_t calculate_memory() const override {
        constexpr size_t stack_frame_size = 32;
        const uint64_t stack_memory = m_max_depth * stack_frame_size;
        const uint64_t table_memory = m_table.size() * m_table[0].size() * sizeof(int);
        return stack_memory + table_memory;
    }

private:
    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        m_call_count++;
        m_current_depth++;
        m_max_depth = std::max(m_max_depth, m_current_depth);

        int &value = m_table[m][n];

        if (value != -1) {
            m_current_depth--;
            return value;
        }

        if (m == 0) {
            m_current_depth--;
            return value = n;
        }

        if (n == 0) {
            m_current_depth--;
            return value = m;
        }

        if (m_s1[m - 1] == m_s2[n - 1]) {
            value = calculate(m - 1, n - 1);
            m_current_depth--;
            return value;
        }

        value = 1 + std::min(Insert(m, n), Delete(m, n));
        m_current_depth--;
        return value;
    }

    int Insert(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m, n - 1);
    }

    int Delete(const int m, const int n) override { // NOLINT(*-no-recursion)
        return calculate(m - 1, n);
    }
};
