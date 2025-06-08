#include <vector>

#include "args.hpp"
#include "text_extractor.hpp"
#include "edit_distance/edit_distance_dp.hpp"
#include "edit_distance/edit_distance_dp_optimized.hpp"
#include "edit_distance/edit_distance_memo.hpp"
#include "uhr/uhr.hpp"

const std::string DATA_DIR = "../data";

int main(const int argc, const char *const *const argv) {
    const ParsedArgs &args = parse_args(argc, argv);

    std::vector<TextExtractor> text_extractors;
    text_extractors.emplace_back(DATA_DIR + "/extract_1.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_2.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_3.txt", args.start_pos);
    text_extractors.emplace_back(DATA_DIR + "/extract_4.txt", args.start_pos);

    for (int i = 0; i < text_extractors.size() - 1; i++) {
        for (int j = i + 1; j < text_extractors.size(); j++) {
            TextExtractor &text_extractor1 = text_extractors[i];
            TextExtractor &text_extractor2 = text_extractors[j];

            const std::string suffix = std::to_string(i) + "_" + std::to_string(j) + ".csv";

            uhr<EditDistanceMemo>(
                DATA_DIR + "/memo_" + suffix,
                args.runs,
                args.lower,
                args.upper,
                args.step,
                text_extractor1,
                text_extractor2
            );

            uhr<EditDistanceDP>(
                DATA_DIR + "/dp_" + suffix,
                args.runs,
                args.lower,
                args.upper,
                args.step,
                text_extractor1,
                text_extractor2
            );

            uhr<EditDistanceDPOptimized>(
                DATA_DIR + "/dp_optimized_" + suffix,
                args.runs,
                args.lower,
                args.upper,
                args.step,
                text_extractor1,
                text_extractor2
            );
        }
    }

    return 0;
}
