cmake_minimum_required(VERSION 3.28)
project(PageRank C)

set(CMAKE_C_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(PageRank function_declarations.h
        main.c
        main_omp.c)

target_link_libraries(PageRank PUBLIC OpenMP::OpenMP_C)
