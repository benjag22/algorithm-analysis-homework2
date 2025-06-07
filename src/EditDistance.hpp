#pragma once

#include <cstdint>
#include <string>

class EditDistance {
protected:
    const std::string &m_s1;
    const std::string &m_s2;

public:
    EditDistance(const std::string &s1, const std::string &s2): m_s1(s1), m_s2(s2) {}

    virtual ~EditDistance() = default;

    virtual int calculate_distance() = 0;

    [[nodiscard]] virtual uint64_t calculate_memory() const = 0;

private:
    virtual int calculate(int m, int n) = 0;

    virtual int Insert(int m, int n) = 0;

    virtual int Delete(int m, int n) = 0;
};
