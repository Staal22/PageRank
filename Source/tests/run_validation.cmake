execute_process(
        COMMAND "${PAGERANK_EXE}"
        simple-webgraph.txt
        simple-webgraph.txt
        1 0.0000001 10
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/pagerank_output.txt
        OUTPUT_VARIABLE out
        ERROR_VARIABLE err
        RESULT_VARIABLE pagerank_result
)

message(STATUS "Output: ${out}")
message(STATUS "Error: ${err}")

# Check if PageRank ran successfully
if (NOT pagerank_result EQUAL 0)
    message(FATAL_ERROR "PageRank execution failed with exit code: ${pagerank_result}")
endif ()

# Run the validation executable to check the results
execute_process(
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/validate_pagerank
        ${CMAKE_CURRENT_BINARY_DIR}/pagerank_output.txt
        RESULT_VARIABLE validation_result
)

# Check if validation was successful
if (NOT validation_result EQUAL 0)
    message(FATAL_ERROR "Validation failed with exit code: ${validation_result}")
endif ()

message(STATUS "PageRank test passed successfully!")
