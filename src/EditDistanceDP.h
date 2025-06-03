#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "EditDistance.h"

using namespace std;

class EditDistanceDP : EditDistance{
private:
    mutable vector<vector<int>> dp;
public:
    EditDistanceDP(const string &S, const string &T ): EditDistance(S,T){
        dp.resize(S.length() + 1,vector<int>(T.length() + 1,-1));

    }

    int calculate(int m, int n) override {
        if (dp[m][n] != -1) {
            return dp[m][n];
        }

        string s = getS();
        string t = getT();

        if (m == 0) {
            dp[m][n] = n;
            return n;
        }

        if (n == 0) {
            dp[m][n] = m;
            return m;
        }

        if (s[m - 1] == t[n - 1]) {
            int replaceCost = Replace(m, n);
            dp[m][n] = replaceCost;
            return dp[m][n];
        }
        int insertCost = Insert(m, n);
        int deleteCost = Delete(m, n);
        int replaceCost = Replace(m, n);

        dp[m][n] = 1 + min({insertCost, deleteCost, replaceCost});

        return dp[m][n];

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

    int CalculateDistance() override {
        return calculate(getS().length(), getT().length());
    }
    size_t getDPSize() const {
        return dp.size() * (dp.empty() ? 0 : dp[0].size());
    }
};
