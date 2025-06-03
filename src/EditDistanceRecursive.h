#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include "EditDistance.h"

using namespace std;
/*
 * Tiempo: O(3^(s.length() + t.length()))
 * espacio: O(s.length() + t.length())
 *
 */
class EditDistanceRecursive : EditDistance {
private:
    int callCount = 0;

public:
    EditDistanceRecursive(const string &S, const string &T) : EditDistance(S, T) {
        callCount = 0;
    }
    /*
     * Por cada posicion (n,m) realizar hasta 3 llamadas recursivas adicionalesen el peor caso.
     */
    int calculate(int m, int n) override {
        callCount++;
        string s = getS();
        string t = getT();

        if (m == 0) return n;

        if (n == 0) return m;

        if (s[m - 1] == t[n - 1]) {
            return calculate(m - 1, n - 1);
        }


        return 1 + min({calculate(m, n - 1), calculate(m - 1, n), calculate(m - 1, n - 1)});

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

    int CalculateDistance() override{
        callCount = 0;
        return calculate(getS().length(), getT().length());
    }
    int getCallCount() const {
        return callCount;
    }
};
