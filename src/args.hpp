#pragma once

#include <cerrno>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

const std::string EXE_NAME = "algorithm_analysis_homework2";

struct ArgValidationResult {
    const bool is_valid = true;
    const std::string message;
};

struct Arg {
    const std::string name;
    const std::string label;
    int64_t value = 0;
    int64_t min = std::numeric_limits<int64_t>::min();
    int64_t max = std::numeric_limits<int64_t>::max();

    std::function<ArgValidationResult()> validator = []() -> ArgValidationResult {
        return {};
    };

    auto operator<=>(const Arg &other) const {
        return value <=> other.value;
    }
};

struct Args {
    Arg runs{
        .name = "-r",
        .label = "runs",
        .value = 32,
        .min = 32,
    };
    Arg lower{
        .name = "-l",
        .label = "lower",
        .value = 8,
        .min = 1,
    };
    Arg upper{
        .name = "-u",
        .label = "upper",
        .value = 2048,
        .min = 1,
        .validator = [this]() -> ArgValidationResult {
            return {upper > lower, upper.label + " must be > " + lower.label};
        },
    };
    Arg step{
        .name = "-s",
        .label = "step",
        .value = 64,
        .min = 1,
    };
    Arg start_pos{
        .name = "-sp",
        .label = "start_pos",
        .value = 0,
        .min = 0,
    };

private:
    typedef std::vector<Arg *> args_list_t;
    args_list_t m_args_list{
        &runs,
        &lower,
        &upper,
        &step,
        &start_pos,
    };
    const std::unordered_map<std::string, Arg *> m_args_map{
        {runs.name, &runs},
        {lower.name, &lower},
        {upper.name, &upper},
        {step.name, &step},
        {start_pos.name, &start_pos},
    };

public:
    [[nodiscard]] Arg *get(const std::string &name) const {
        return m_args_map.contains(name) ? m_args_map.at(name) : nullptr;
    }

    [[nodiscard]] size_t size() const {
        return m_args_list.size();
    }

    [[nodiscard]] std::string usage_string() const {
        std::stringstream ss;

        ss << "Usage: " + EXE_NAME + " [options]\n\n"
                << "Options:\n\n";

        for (const auto &arg : m_args_list) {
            ss << "  " << arg->name << "\t: " << arg->label << "\n"
                    << "\t    default: " << arg->value << "\n";

            if (arg->min != std::numeric_limits<int64_t>::min()) {
                ss << "\t    min: " << arg->min << "\n";
            }

            if (arg->max != std::numeric_limits<int64_t>::max()) {
                ss << "\t    max: " << arg->max << "\n";
            }

            ss << "\n";
        }

        return ss.str();
    }

    args_list_t::iterator begin() {
        return m_args_list.begin();
    }

    args_list_t::iterator end() {
        return m_args_list.begin();
    }
};

struct ParsedArgs {
    int64_t runs;
    int64_t lower;
    int64_t upper;
    int64_t step;
    int64_t start_pos;

    explicit ParsedArgs(const Args &args):
        runs(args.runs.value),
        lower(args.lower.value),
        upper(args.upper.value),
        step(args.step.value),
        start_pos(args.start_pos.value) {}
};

inline ParsedArgs parse_args(const int argc, const char *const *const argv) {
    using std::cerr, std::endl, std::exit;

    Args args;

    for (int i = 1; i < argc; i += 2) {
        Arg *arg_ptr = args.get(argv[i]);

        if (arg_ptr == nullptr) {
            cerr << EXE_NAME << ": " << "invalid option " << argv[i] << "\n\n"
                    << args.usage_string() << endl;
            exit(EXIT_FAILURE);
        }

        if (i + 1 >= argc) {
            break;
        }

        auto &[name, label, value, min, max, validator] = *arg_ptr;

        errno = 0;
        char *end;
        const char *string = argv[i + 1];
        const int64_t parsed_value = std::strtol(string, &end, 10);

        if (string == end || errno == ERANGE) {
            cerr << EXE_NAME << ": " << "invalid option value " << argv[i + 1] << "\n\n"
                    << args.usage_string() << endl;
            exit(EXIT_FAILURE);
        }

        value = parsed_value;

        if (value < min) {
            cerr << EXE_NAME << ": " << label << " must be >= " << min << "\n\n"
                    << args.usage_string() << endl;
            exit(EXIT_FAILURE);
        }

        if (value > max) {
            cerr << EXE_NAME << ": " << label << " must be <= " << max << "\n\n"
                    << args.usage_string() << endl;
            exit(EXIT_FAILURE);
        }
    }

    for (const auto *arg : args) {
        const auto &[is_valid, message] = arg->validator();

        if (!is_valid) {
            cerr << EXE_NAME << ": " << message << "\n\n"
                    << args.usage_string() << endl;
            exit(EXIT_FAILURE);
        }
    }

    return ParsedArgs(args);
}
