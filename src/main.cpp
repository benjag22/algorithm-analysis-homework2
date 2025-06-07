#include "ExperimentRunner.hpp"

int main() {
    ExperimentRunner experiment_runner("data/pg76210.txt", 1, 100, 4);
    experiment_runner.run_all_tests();
    return 0;
}
