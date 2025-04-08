cmake_minimum_required(VERSION 3.28)
project(PageRank C)

set(CMAKE_C_STANDARD 17)

enable_testing()

find_package(OpenMP)
if (OpenMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
else ()
    message(STATUS "OpenMP not found - building without parallel support")
endif ()

if (UNIX)
    find_library(MATH_LIBRARY m)
else ()
    # On Windows, the math functions are in the C runtime
    set(MATH_LIBRARY "")
endif ()

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
# Set compiler flags with platform checks
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -fomit-frame-pointer -flto")
elseif (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2 /GL")
endif ()

add_executable(PageRank function_declarations.h
        main.c
        main_omp.c)

if (OpenMP_FOUND)
    target_link_libraries(PageRank PUBLIC OpenMP::OpenMP_C)
endif ()

if (MATH_LIBRARY)
    target_link_libraries(PageRank PUBLIC ${MATH_LIBRARY})
endif ()

add_executable(validate_pagerank tests/validate_pagerank.c)

add_test(
        NAME pagerank_validation
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_validation.cmake
)

if (DEFINED ENV{GITHUB_ACTIONS})
    set_tests_properties(pagerank_validation PROPERTIES
            ENVIRONMENT "GRAPH_DIR=../Graphs/"
    )
endif ()
