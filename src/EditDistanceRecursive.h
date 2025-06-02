#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include "EditDistance.h"

using namespace std;

class EditDistanceRecursive : EditDistance {
private:
    mutable int callCount = 0;

public:
    EditDistanceRecursive(const string &S, const string &T) : EditDistance(S, T) {
        callCount = 0;
    }

    int calculate(int m, int n) override {
        callCount++;
        string s = getS();
        string t = getT();

        if (m == 0) return n;

        if (n == 0) return m;

        if (s[m - 1] == t[n - 1]) {
            return Replace(m, n);
        }


        return 1 + min({Insert(m, n), Delete(m, n), Replace(m, n)});

    }

    int Insert(int m, int n) override {
        return calculate(m, n - 1);
    }

    int Delete(int m, int n) override {
        return calculate(m - 1, n);
    }

    int Replace(int m, int n) override {
        return calculate(m - 1, n - 1);
    }

    int CalculateDistante() override{
        callCount = 0;
        return calculate(getS().length(), getT().length());
    }
    int getCallCount() const {
        return callCount;
    }
};
