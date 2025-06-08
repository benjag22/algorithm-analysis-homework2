cmake_minimum_required(VERSION 3.30)
project(algorithm_analysis_homework2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(
    algorithm_analysis_homework2
    src/main.cpp
    src/args.hpp
    src/edit_distance/edit_distance.hpp
    src/edit_distance/edit_distance_dp.hpp
    src/edit_distance/edit_distance_dp_optimized.hpp
    src/edit_distance/edit_distance_memo.hpp
    src/edit_distance/edit_distance_recursive.hpp
    src/uhr/uhr.hpp
    src/uhr/uhr_utils.hpp
    src/text_extractor.hpp
)
