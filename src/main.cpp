#include "args.hpp"
#include "ExperimentRunner.hpp"

int main(const int argc, const char *const *const argv) {
    const ParsedArgs &args = parse_args(argc, argv);

    ExperimentRunner experiment_runner(
        "data/pg76210.txt",
        args.start_pos,
        args.extract_length,
        args.number_extracts
    );

    experiment_runner.run_all_tests();

    return 0;
}
