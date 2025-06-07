cmake_minimum_required(VERSION 3.30)
project(algorithm_analysis_homework2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

add_executable(
    algorithm_analysis_homework2
    src/main.cpp
    src/EditDistance.hpp
    src/EditDistanceDP.hpp
    src/EditDistanceDPOptimized.hpp
    src/EditDistanceMemo.hpp
    src/EditDistanceRecursive.hpp
    src/TextExtractor.hpp
    src/PerformanceMeter.hpp
    src/ExperimentRunner.hpp
)
