#pragma once

#include <algorithm>
#include <string>

#include "edit_distance.hpp"

class EditDistanceRecursive final : EditDistance {
    uint64_t m_call_count = 0;
    uint64_t m_current_depth = 0;
    uint64_t m_max_depth = 0;

public:
    EditDistanceRecursive(const std::string &s1, const std::string &s2) : EditDistance(s1, s2) {}

    int calculate_distance() override {
        m_call_count = 0;
        m_current_depth = 0;
        m_max_depth = 0;
        return calculate(m_s1.length(), m_s2.length());
    }

    [[nodiscard]] uint64_t calculate_memory() const override {
        constexpr uint64_t stack_frame_size = 32;
        return m_call_count * stack_frame_size;
    }

private:
    int calculate(const int m, const int n) override { // NOLINT(*-no-recursion)
        m_call_count++;
        m_current_depth++;
        m_max_depth = std::max(m_max_depth, m_current_depth);

        if (m == 0) {
            m_current_depth--;
            return n;
        }

        if (n == 0) {
            m_current_depth--;
            return m;
        }

        if (m_s1[m - 1] == m_s2[n - 1]) {
            const int value = calculate(m - 1, n - 1);
            m_current_depth--;
            return value;
        }

        const int value = 1 + std::min(Insert(m, n), Delete(m, n));
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
