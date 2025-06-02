#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class TextExtractor {
public:

    static string extractLinesFromFile(const string& filePath, int startLine, int endLine) {
        if (endLine < startLine || startLine <= 0) {
            cerr << "Error: Rango de lineas invalido (" << startLine << "-" << endLine << ")" << endl;
            return "";
        }


        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Error: No se pudo abrir el archivo: " << filePath << endl;
            return "";
        }

        string result = "";
        string line;
        int currentLine = 0;
        const int MAX_LINES = 50000;

        while (getline(file, line) && currentLine < MAX_LINES) {
            currentLine++;

            if (currentLine >= startLine && currentLine <= endLine) {

                result += line;
                if (currentLine < endLine) {
                    result += "\n";
                }
            }
            if (currentLine > endLine) {
                break;
            }
        }

        file.close();
        cout<<result<<endl;
        return result;
    }

    static vector<string> extractMultipleTexts(const string& filePath, int startLine,
                                               int linesPerExtract, int numExtracts) {
        vector<string> texts;
        for (int i = 0; i < numExtracts; i++) {
            int start = startLine + i * (linesPerExtract + 10);
            cout<<start<<endl;
            string text = extractLinesFromFile(filePath, start, start + linesPerExtract - 1);
            if (!text.empty()) {
                texts.push_back(text);
            }
        }
        return texts;
    }
};