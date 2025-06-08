#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class TextExtractor {
    std::ifstream m_file;
    uint64_t m_file_size;

public:
    explicit TextExtractor(const std::string &file_path) : m_file(file_path) {
        if (!m_file.is_open() || !m_file.good() || m_file.bad() || m_file.fail() || m_file.eof()) {
            std::cerr << "Error: Could not open file: " << file_path << std::endl;
            std::exit(EXIT_FAILURE);
        }

        m_file.seekg(0, std::ios::end);
        m_file_size = m_file.tellg();
        m_file.seekg(0, std::ios::beg);
    }

    ~TextExtractor() = default;

    std::vector<std::string> extract_multiple_texts(
        const int start_pos,
        const int extract_length,
        const int number_extracts
    ) {
        if (start_pos >= m_file_size) {
            std::cerr << "error: start pos " << start_pos << " is out of range\n"
                    << "file size is " << m_file_size << std::endl;
            std::exit(EXIT_FAILURE);
        }

        const uint64_t total_length = start_pos + static_cast<uint64_t>(extract_length) * number_extracts;
        if (total_length > m_file_size) {
            std::cerr << "error: total length " << total_length << " is out of range\n"
                    << "file size is " << m_file_size << std::endl;
            std::exit(EXIT_FAILURE);
        }

        m_file.seekg(start_pos, std::ios::beg);

        std::vector<std::string> texts(number_extracts);

        for (int i = 0; i < number_extracts; i++) {
            std::string text;

            for (int j = 0; j < extract_length; j++) {
                text += static_cast<char>(m_file.get());
            }

            texts[i] = text;
        }

        return texts;
    }
};
