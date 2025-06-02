#include <iostream>
#include "ExperimentRunner.h"
using namespace std;

int main() {
    ExperimentRunner runTest("src/data/pg76210.txt", 1, 100);
    runTest.runAllTests();
    return 0;
}