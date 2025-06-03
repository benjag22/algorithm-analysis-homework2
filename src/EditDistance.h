#pragma once
#include <iostream>
#include <string>

using namespace std;

class EditDistance{
private:
    string T;
    string S;

public:
    EditDistance(const string& S, const string& T) {
        this->S = S;
        this->T = T;
    }
    virtual int calculate(int m, int n)= 0;

    virtual int Insert(int m, int n) = 0;

    virtual int Delete(int m, int n) = 0;

    virtual int Replace(int m, int n) = 0;
    virtual int CalculateDistance() = 0;
    string getS() const { return S; }
    string getT() const { return T; }

};