#pragma once

#include <string>
#include <utility>

class EditDistance {
protected:
    const std::string m_s1;
    const std::string m_s2;

public:
    virtual ~EditDistance() = default;

    EditDistance(std::string s1, std::string s2): m_s1(std::move(s1)), m_s2(std::move(s2)) {}

    virtual int calculate(int m, int n) = 0;

    virtual int Insert(int m, int n) = 0;

    virtual int Delete(int m, int n) = 0;

    virtual int calculate_distance() = 0;
};
