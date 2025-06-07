#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class TextExtractor {
    static constexpr int MAX_LINES = 50'000;
    std::ifstream m_file;

public:
    explicit TextExtractor(const std::string &file_path) : m_file(file_path) {
        if (!m_file.is_open()) {
            std::cerr << "Error: Could not open file: " << file_path << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    ~TextExtractor() = default;

    std::vector<std::string> extract_multiple_texts(
        const int start_line,
        const int lines_per_extract,
        const int num_extracts
    ) {
        m_file.seekg(0, std::ios::beg);
        int current_line = 1;

        std::vector<std::string> texts;

        for (int i = 0; i < num_extracts; i++) {
            const int start = start_line + i * lines_per_extract;
            const int end = start + lines_per_extract - 1;
            const std::string &text = extract_lines_from_file(current_line, start, end);

            if (!text.empty()) {
                texts.push_back(text);
            }
        }

        return texts;
    }

private:
    std::string extract_lines_from_file(int &current_line, const int start_line, const int end_line) {
        if (end_line < start_line || start_line <= 0) {
            std::cerr << "Error: Invalid lines range (" << start_line << "-" << end_line << ")" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::string result;
        std::string line;

        while (current_line <= end_line && current_line < MAX_LINES && std::getline(m_file, line)) {
            if (current_line >= start_line) {
                result += line;

                if (current_line < end_line) {
                    result += "\n";
                }
            }

            current_line++;
        }

        return result;
    }
};
