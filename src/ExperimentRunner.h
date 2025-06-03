#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <functional>
#include "PerformanceMeter.h"
#include "TextExtractor.h"
#include "EditDistanceDP.h"
#include "EditDistanceRecursive.h"

using namespace std;

class ExperimentRunner {
private:
    vector<string> texts;
    vector<vector<vector<PerformanceMeter::Result>>> results;
public:
    ExperimentRunner(const string& filePath, int startLine, int linesPerExtract) {
        // Extrae de un .txt 4 extractos de linesPerExtract lineas
        texts = TextExtractor::extractMultipleTexts(filePath, startLine, linesPerExtract, 4);

        //[4 textos origen][4 textos destino][4 algoritmos].
        results.resize(4, vector<vector<PerformanceMeter::Result>>(4, vector<PerformanceMeter::Result>(4)));
    }

    void runAllTests(){
        run12TestForRecursiveDP();
        run12TestForRecursive();
    }

    void run12TestForRecursiveDP() {
        cout << "\n=== Ejecutando 12 pruebas para la version recursiva con DP ===" << endl;
        int count = 0;
        for (int i = 0; i < texts.size(); i++) {

            for (int j = 0; j < texts.size(); j++) {
                if (i != j) {
                    runTestRecursiveDP(i, j);
                }
            }
        }
    }

    void run12TestForRecursive() {
        cout << "\n=== Ejecutando pruebas para la version recursiva normal ===" << endl;
        for (int i = 0; i < texts.size(); i++) {

            for (int j = 0; j < texts.size(); j++) {
                if (i != j) {
                    runTestRecursive(i, j);
                }
            }
        }
    }

    void runTestRecursive(int sourceIdx, int targetIdx) {

        // Algoritmo 3.A: Recursivo

        const string& source = texts[sourceIdx];
        const string& target = texts[targetIdx];

        EditDistanceRecursive recursive(source, target);

        results[sourceIdx][targetIdx][0] = PerformanceMeter::measure<EditDistanceRecursive>(recursive,[](const EditDistanceRecursive& alg) {return alg.getCallCount() * 64;});

        cout << "  Recursivo: Distancia = " << results[sourceIdx][targetIdx][0].distance
             << ", Tiempo = " << results[sourceIdx][targetIdx][0].timeMs << "ms"
             << ", Memoria ≈ " << results[sourceIdx][targetIdx][0].memoryBytes << " bytes" << endl;
    }
    void runTestRecursiveDP(int sourceIdx, int targetIdx) {
        // Algoritmo 3.C: DP

        const string& source = texts[sourceIdx];
        const string& target = texts[targetIdx];

        EditDistanceDP dp(source, target);
        results[sourceIdx][targetIdx][2] = PerformanceMeter::measure<EditDistanceDP>(dp,
                                                                                     [](const EditDistanceDP& alg) {
                                                                                         return alg.getDPSize() * sizeof(int);
                                                                                     }
        );

        cout << "  DP: Distancia = " << results[sourceIdx][targetIdx][2].distance
             << ", Tiempo = " << results[sourceIdx][targetIdx][2].timeMs << "ms"
             << ", Memoria ≈ " << results[sourceIdx][targetIdx][2].memoryBytes << " bytes" << endl;
    }
};

