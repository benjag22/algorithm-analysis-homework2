#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

class TextExtractor {
    std::ifstream m_file;
    uint64_t m_file_size;
    const uint64_t m_start_pos;

public:
    explicit TextExtractor(const std::string &file_path, const uint64_t start_pos):
        m_file(file_path),
        m_start_pos(start_pos) {
        if (!m_file.is_open() || !m_file.good() || m_file.bad() || m_file.fail() || m_file.eof()) {
            std::cerr << "Error: Could not open file: " << file_path << std::endl;
            std::exit(EXIT_FAILURE);
        }

        m_file.seekg(0, std::ios::end);
        m_file_size = m_file.tellg();
        m_file.seekg(0, std::ios::beg);

        if (start_pos >= m_file_size) {
            std::cerr << "error: start pos " << start_pos << " is out of range\n"
                    << "file size is " << m_file_size << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    TextExtractor(TextExtractor &&) noexcept = default;

    ~TextExtractor() = default;

    std::string extract(const uint64_t length) {
        m_file.seekg(m_start_pos, std::ios::beg);

        const uint64_t limit = std::min(length, m_file_size);
        std::string text;

        for (uint64_t i = 0; i < limit; i++) {
            text += static_cast<char>(m_file.get());
        }

        return text;
    }
};
