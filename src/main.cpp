#include <iostream>
#include "EditDistanceRecursive.h"
#include "EditDistanceDP.h"
using namespace std;

int main() {
    string s1 = "abcd";
    string s2 = "bcfe";

    EditDistanceRecursive ed1(s1,s2);
    EditDistanceDP ed2(s1,s2);

    cout << ed1.CalculateDistante()<< endl;
    cout << ed2.CalculateDistante()<< endl;
    return 0;
}