#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "EditDistance.h"

using namespace std;

class EditDistanceDP : EditDistance {
private:
    mutable vector<vector<int>> dp;

public:
    EditDistanceDP(const string &S, const string &T) : EditDistance(S, T) {
        dp.resize(S.length() + 1, vector<int>(T.length() + 1, -1));
    }

    int calculate(int m, int n) override {
        if (dp[m][n] != -1) {
            return dp[m][n];
        }

        string s = getS();
        string t = getT();

        if (m == 0) return dp[m][n] = n;
        if (n == 0) return dp[m][n] = m;

        if (s[m - 1] == t[n - 1]) {
            return dp[m][n] = calculate(m - 1, n - 1);
        }

        int insertCost = calculate(m, n - 1);
        int deleteCost = calculate(m - 1, n);

        return dp[m][n] = 1 + min(insertCost, deleteCost);
    }

    int Insert(int m, int n) override {
        return calculate(m, n - 1);
    }

    int Delete(int m, int n) override {
        return calculate(m - 1, n);
    }

    int calculateIterative() const {
        string s = getS();
        string t = getT();
        int m = s.length();
        int n = t.length();

        vector<vector<int>> table(m + 1, vector<int>(n + 1, 0));

        for (int i = 0; i <= m; ++i) {
            for (int j = 0; j <= n; ++j) {
                if (i == 0) {
                    table[i][j] = j;
                } else if (j == 0) {
                    table[i][j] = i;
                } else if (s[i - 1] == t[j - 1]) {
                    table[i][j] = table[i - 1][j - 1];
                } else {
                    table[i][j] = 1 + min(table[i - 1][j], table[i][j - 1]);
                }
            }
        }

        return table[m][n];
    }

    int CalculateDistance() override {
        return calculateIterative();
    }

    size_t getDPSize() const {
        return dp.size() * (dp.empty() ? 0 : dp[0].size());
    }
};