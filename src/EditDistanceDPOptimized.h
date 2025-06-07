#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "EditDistance.h"

using namespace std;

class EditDistanceDPOptimized : public EditDistance {
public:
    EditDistanceDPOptimized(const string &S, const string &T) : EditDistance(S, T) {}

    int calculateIterative() const {
        const string &s = getS();
        const string &t = getT();
        int m = s.length();
        int n = t.length();

        vector<int> prev(n + 1, 0);
        vector<int> curr(n + 1, 0);

        for (int j = 0; j <= n; ++j) {
            prev[j] = j;
        }

        for (int i = 1; i <= m; ++i) {
            curr[0] = i;

            for (int j = 1; j <= n; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    curr[j] = prev[j - 1];
                } else {
                    curr[j] = 1 + min(
                        curr[j - 1],
                        prev[j]
                    );
                }
            }

            swap(prev, curr);
        }

        return prev[n];
    }

    int Insert(int m, int n) override {
        return -1;
    }

    int Delete(int m, int n) override {
        return -1;
    }

    int calculate(int m, int n) override {
        return calculateIterative();
    }

    int CalculateDistance() override {
        return calculateIterative();
    }
};