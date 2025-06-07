#include <iostream>
#include "EditDistanceRecursive.h"
#include "EditDistanceMemoized.h"

int main() {
    std::string s = "horse";
    std::string t = "ros";

    // --- Prueba Recursiva ---
    EditDistanceRecursive edRecursive(s, t);
    int distanceRecursive = edRecursive.CalculateDistance();

    std::cout << "===== Metodo Recursivo Puro =====" << std::endl;
    std::cout << "Cadena S: " << s << std::endl;
    std::cout << "Cadena T: " << t << std::endl;
    std::cout << "Distancia de edicion: " << distanceRecursive << std::endl;
    std::cout << "Numero de llamadas recursivas: " << edRecursive.getCallCount() << std::endl;

    std::cout << std::endl;

    // --- Prueba con Memoización ---
    EditDistanceMemoized edMemo(s, t);
    int distanceMemo = edMemo.CalculateDistance();

    std::cout << "===== Metodo con Memoizacion =====" << std::endl;
    std::cout << "Cadena S: " << s << std::endl;
    std::cout << "Cadena T: " << t << std::endl;
    std::cout << "Distancia de edicion: " << distanceMemo << std::endl;
    std::cout << "Numero de llamadas recursivas: " << edMemo.getCallCount() << std::endl;

    return 0;
}
