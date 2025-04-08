#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function_declarations.h"

int omp_pagerank(int argc, char** argv)
{
    printf("--- PageRank OMP ---\n");
    printf("Commandline arguments:\n");
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");

    // Read arguments
    const char* graph_name = argv[1];     // "web-stanford.txt";
    const double damping = atof(argv[2]); // 0.85
    const double epsilon = atof(argv[3]); // 0.0000001;
    const int top_pages = atoi(argv[4]);  // 10;

    // Concatenate string to include full path
    const char* graphs_path = "../../Graphs/";
    const size_t graph_path_length = strlen(graphs_path) + strlen(argv[1]) + 1;
    char* graph_path = malloc(graph_path_length);
    strcpy(graph_path, graphs_path);
    strcat(graph_path, graph_name);

    int graph_nodes;
    int* rows;
    int* cols;
    double* vals;
    read_graph_from_file2(graph_path, &graph_nodes, &rows, &cols, &vals);
    printf("Number of pages: %d \n", graph_nodes);

    double* scores = malloc(graph_nodes * sizeof(double));

    // Implemented OpenMP directly in base function
    PageRank_iterations2(graph_nodes, rows, cols, vals, damping, epsilon, scores);

    // Implemented OpenMP directly in base function
    top_n_webpages(graph_nodes, scores, top_pages);

    free(scores);

    return 0;
}
