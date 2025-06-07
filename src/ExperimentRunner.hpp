#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "EditDistanceDP.hpp"
#include "EditDistanceDPOptimized.hpp"
#include "EditDistanceMemo.hpp"
#include "EditDistanceRecursive.hpp"
#include "PerformanceMeter.hpp"
#include "TextExtractor.hpp"

class ExperimentRunner {
    std::vector<std::string> m_texts;
    std::vector<std::vector<std::vector<PerformanceMeter::Result>>> m_results;

public:
    ExperimentRunner(const std::string &file_path, const int start_line, const int lines_per_extract) {
        // Extrae de un .txt 4 extractos de lines_per_extract lineas
        m_texts = TextExtractor::extract_multiple_texts(file_path, start_line, lines_per_extract, 4);

        // [4 textos origen][4 textos destino][4 algoritmos].
        m_results.resize(4, std::vector(4, std::vector<PerformanceMeter::Result>(4)));
    }

    void run_all_tests() {
        // run_recursive_tests();
        run_memo_tests();
        run_dp_tests();
        run_dp_optimized_tests();
    }

private:
    void run_recursive_tests() {
        std::cout << "\n=== Ejecutando 12 pruebas para la version recursiva normal ===" << std::endl;

        for (int i = 0; i < m_texts.size(); i++) {
            for (int j = 0; j < m_texts.size(); j++) {
                if (i != j) {
                    run_recursive_test(i, j);
                }
            }
        }
    }

    void run_memo_tests() {
        std::cout << "\n=== Ejecutando 12 pruebas para la version recursiva memoizada ===" << std::endl;

        for (int i = 0; i < m_texts.size(); i++) {
            for (int j = 0; j < m_texts.size(); j++) {
                if (i != j) {
                    run_memo_test(i, j);
                }
            }
        }
    }

    void run_dp_tests() {
        std::cout << "\n=== Ejecutando 12 pruebas para la version con DP ===" << std::endl;

        for (int i = 0; i < m_texts.size(); i++) {
            for (int j = 0; j < m_texts.size(); j++) {
                if (i != j) {
                    run_dp_test(i, j);
                }
            }
        }
    }

    void run_dp_optimized_tests() {
        std::cout << "\n=== Ejecutando 12 pruebas para la version con DP optimizada ===" << std::endl;

        for (int i = 0; i < m_texts.size(); i++) {
            for (int j = 0; j < m_texts.size(); j++) {
                if (i != j) {
                    run_dp_optimized_test(i, j);
                }
            }
        }
    }

    void run_recursive_test(const int source_idx, const int target_idx) {
        // Algoritmo 3.A: Recursivo

        const std::string &source = m_texts[source_idx];
        const std::string &target = m_texts[target_idx];

        EditDistanceRecursive recursive(source, target);

        m_results[source_idx][target_idx][0] = PerformanceMeter::measure<EditDistanceRecursive>(
            recursive,
            [](const EditDistanceRecursive &alg) {
                return alg.get_call_count() * 64;
            }
        );

        std::cout << "  Recursivo: Distancia = " << m_results[source_idx][target_idx][0].distance
                << ", Tiempo = " << m_results[source_idx][target_idx][0].time_ms << "ms"
                << ", Memoria ≈ " << m_results[source_idx][target_idx][0].memory_bytes << " bytes" << std::endl;
    }

    void run_memo_test(const int source_idx, const int target_idx) {
        // Algoritmo 3.B: Recursivo Memoizado

        const std::string &source = m_texts[source_idx];
        const std::string &target = m_texts[target_idx];

        EditDistanceMemo recursive(source, target);

        m_results[source_idx][target_idx][0] = PerformanceMeter::measure<EditDistanceMemo>(
            recursive,
            [](const EditDistanceMemo &alg) {
                return static_cast<size_t>(alg.get_call_count()) * 64;
            }
        );

        std::cout << "  Recursivo: Distancia = " << m_results[source_idx][target_idx][0].distance
                << ", Tiempo = " << m_results[source_idx][target_idx][0].time_ms << "ms"
                << ", Memoria ≈ " << m_results[source_idx][target_idx][0].memory_bytes << " bytes" << std::endl;
    }

    void run_dp_test(const int source_idx, const int target_idx) {
        // Algoritmo 3.C: DP

        const std::string &source = m_texts[source_idx];
        const std::string &target = m_texts[target_idx];

        EditDistanceDP dp(source, target);
        m_results[source_idx][target_idx][2] = PerformanceMeter::measure<EditDistanceDP>(
            dp,
            [](const EditDistanceDP &alg) {
                return alg.get_dp_size() * sizeof(int);
            }
        );

        std::cout << "  DP: Distancia = " << m_results[source_idx][target_idx][2].distance
                << ", Tiempo = " << m_results[source_idx][target_idx][2].time_ms << "ms"
                << ", Memoria ≈ " << m_results[source_idx][target_idx][2].memory_bytes << " bytes" << std::endl;
    }

    void run_dp_optimized_test(const int source_idx, const int target_idx) {
        // Algoritmo 3.D: DP Optimizado

        const std::string &source = m_texts[source_idx];
        const std::string &target = m_texts[target_idx];

        EditDistanceDPOptimized dp_optimizado(source, target);
        m_results[source_idx][target_idx][2] = PerformanceMeter::measure<EditDistanceDPOptimized>(
            dp_optimizado,
            [](const EditDistanceDPOptimized &alg) {
                return 0;
            }
        );

        std::cout << "  DP Optimizado: Distancia = " << m_results[source_idx][target_idx][2].distance
                << ", Tiempo = " << m_results[source_idx][target_idx][2].time_ms << "ms"
                << ", Memoria ≈ " << m_results[source_idx][target_idx][2].memory_bytes << " bytes" << std::endl;
    }
};
