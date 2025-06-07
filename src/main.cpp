#include "ExperimentRunner.hpp"

int main() {
    ExperimentRunner run_test("data/pg76210.txt", 1, 100);
    run_test.run_all_tests();
    return 0;
}
