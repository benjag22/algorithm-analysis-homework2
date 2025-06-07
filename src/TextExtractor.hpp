#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class TextExtractor {
public:
    static std::string extract_lines_from_file(const std::string &file_path, const int start_line, const int end_line) {
        if (end_line < start_line || start_line <= 0) {
            std::cerr << "Error: Rango de lineas invalido (" << start_line << "-" << end_line << ")" << std::endl;
            return "";
        }

        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo: " << file_path << std::endl;
            return "";
        }

        std::string result;
        std::string line;
        int current_line = 0;
        constexpr int MAX_LINES = 50'000;

        while (getline(file, line) && current_line < MAX_LINES) {
            current_line++;

            if (current_line >= start_line && current_line <= end_line) {
                result += line;

                if (current_line < end_line) {
                    result += "\n";
                }
            }

            if (current_line > end_line) {
                break;
            }
        }

        file.close();
        return result;
    }

    static std::vector<std::string> extract_multiple_texts(
        const std::string &file_path,
        const int start_line,
        const int lines_per_extract,
        const int num_extracts
    ) {
        std::vector<std::string> texts;

        for (int i = 0; i < num_extracts; i++) {
            const int start = start_line + i * lines_per_extract;
            std::string text = extract_lines_from_file(file_path, start, start + lines_per_extract - 1);

            if (!text.empty()) {
                texts.push_back(text);
            }
        }

        return texts;
    }
};
