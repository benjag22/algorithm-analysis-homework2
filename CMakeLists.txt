cmake_minimum_required(VERSION 3.30)
project(algorithm_analysis_homework2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

add_executable(
    algorithm_analysis_homework2
    src/main.cpp
    src/args.hpp
    src/edit_distance/EditDistance.hpp
    src/edit_distance/EditDistanceDP.hpp
    src/edit_distance/EditDistanceDPOptimized.hpp
    src/edit_distance/EditDistanceMemo.hpp
    src/edit_distance/EditDistanceRecursive.hpp
    src/uhr/uhr.hpp
    src/uhr/uhr_utils.hpp
    src/TextExtractor.hpp
)
