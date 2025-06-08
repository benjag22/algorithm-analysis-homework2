#include <vector>

#include "args.hpp"
#include "text_extractor.hpp"
#include "edit_distance/edit_distance.hpp"
#include "edit_distance/edit_distance_dp.hpp"
#include "edit_distance/edit_distance_dp_optimized.hpp"
#include "edit_distance/edit_distance_memo.hpp"
#include "edit_distance/edit_distance_recursive.hpp"
#include "uhr/uhr.hpp"

const std::string DATA_DIR = "../data";

template <typename ED, std::enable_if_t<std::is_base_of_v<EditDistance, ED>>* = nullptr>
void run_uhr(
    const ParsedArgs &args,
    const char *name,
    std::vector<TextExtractor> &text_extractors,
    const int i,
    const int j
) {
    TextExtractor &text_extractor1 = text_extractors[i];
    TextExtractor &text_extractor2 = text_extractors[j];

    const std::string suffix = "_" + std::to_string(i + 1) + "_" + std::to_string(j + 1) + ".csv";

    uhr<ED>(
        DATA_DIR + "/" + name + suffix,
        args.runs,
        args.lower,
        args.upper,
        args.step,
        text_extractor1,
        text_extractor2
    );
}

int main(const int argc, const char *const *const argv) {
    const ParsedArgs &args = parse_args(argc, argv);
    const ParsedArgs recursive_args = {args.runs, 1, 32, 1, args.start_pos};

    std::vector<TextExtractor> text_extractors;
    text_extractors.emplace_back(DATA_DIR + "/extract_1.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_2.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_3.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_4.txt", args.start_pos);

    for (int i = 0; i < text_extractors.size() - 1; i++) {
        for (int j = i + 1; j < text_extractors.size(); j++) {
            run_uhr<EditDistanceRecursive>(recursive_args, "recursive", text_extractors, i, j);
            run_uhr<EditDistanceMemo>(args, "memo", text_extractors, i, j);
            run_uhr<EditDistanceDP>(args, "dp", text_extractors, i, j);
            run_uhr<EditDistanceDPOptimized>(args, "dp_optimized", text_extractors, i, j);
        }
    }

    return 0;
}
