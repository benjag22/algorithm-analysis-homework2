#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "EditDistance.h"

using namespace std;

class EditDistanceMemo : public EditDistance {
private:
    int callCount = 0;
    vector<vector<int>> memori;

public:
    EditDistanceMemo(const string &S, const string &T) : EditDistance(S, T) {
        int m = S.length();
        int n = T.length();
        memori = vector<vector<int>>(m + 1, vector<int>(n + 1, -1));
    }

    int calculate(int m, int n) override {
        callCount++;
        string s = getS();
        string t = getT();

        if (memori[m][n] != -1) {
            return memori[m][n];
        }

        if (m == 0) return memori[m][n] = n;

        if (n == 0) return memori[m][n] = m;

        if (s[m - 1] == t[n - 1]) {
            return memori[m][n] = calculate(m - 1, n - 1);
        }

        return memori[m][n] = 1 + min(calculate(m, n - 1), calculate(m - 1, n));
    }

    int Insert(int m, int n) override {
        return calculate(m, n - 1);
    }

    int Delete(int m, int n) override {
        return calculate(m - 1, n);
    }

    int CalculateDistance() override {
        callCount = 0;
        return calculate(getS().length(), getT().length());
    }

    int getCallCount() const {
        return callCount;
    }
};
