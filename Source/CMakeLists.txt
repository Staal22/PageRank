cmake_minimum_required(VERSION 3.28)
project(PageRank C)

set(CMAKE_C_STANDARD 17)

find_package(OpenMP REQUIRED)

find_library(MATH_LIBRARY m)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -fomit-frame-pointer -flto")

add_executable(PageRank function_declarations.h
        main.c
        main_omp.c)

target_link_libraries(PageRank PUBLIC OpenMP::OpenMP_C ${MATH_LIBRARY})
