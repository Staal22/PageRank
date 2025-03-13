#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "function_declarations.h"

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    char* example_graph_path = "../../Graphs/simple-webgraph.txt";
    char* small_graph_path = "../../Graphs/100nodes_graph.txt";
    char* large_graph_path = "../../Graphs/web-stanford.txt";

    const double damping = 1.0;
    const double epsilon = 0.000001;

    // --- Small graph ---
    int small_graph_nodes;
    double** small_graph;
    read_graph_from_file1(example_graph_path, &small_graph_nodes, &small_graph);
    // printf("Number of pages: %d \n", small_graph_nodes);
    // for (int i = 0; i < small_graph_nodes; i++)
    // {
    //     for (int j = 0; j < small_graph_nodes; j++)
    //     {
    //         printf(" %lf", small_graph[i][j]);
    //     }
    //     printf("\n");
    // }

    double* scores = malloc(small_graph_nodes * sizeof(double));
    PageRank_iterations1(small_graph_nodes, small_graph, damping, epsilon, scores);
    // printf("PageRank scores: ");
    // for (int i = 0; i < small_graph_nodes; ++i)
    // {
    //     printf(" %lf", scores[i]);
    // }

    const int top = small_graph_nodes;
    top_n_webpages(small_graph_nodes, scores, top);

    free(scores);

    int large_graph_nodes;
    int* rows;
    int* cols;
    double* vals;
    // read_graph_from_file2(large_graph_path, &large_graph_nodes, &rows, &cols, &vals);
    // printf("Number of pages: %d \n", pages1);
    // for (int i = 0; i < pages1; i++)
    // {
    //     for (int j = 0; j < pages1; j++)
    //     {
    //         printf(" %lf", graph1[i][j]);
    //     }
    //     printf("\n");
    // }

    // const int error = omp_pagerank(argc, argv);
    // if (error != 0)
    // {
    //     printf("OMP PageRank failed\n");
    // }

    return 0;
}

void read_graph_from_file1(char* filename, int* N, double*** hyperlink_matrix)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Read the file to find the number of nodes
    *N = 0;
    int max_buffer_size = 1024;
    char* line = malloc(max_buffer_size * sizeof(char));
    if (line == NULL)
    {
        printf("Memory allocation failed for line buffer\n");
        fclose(file);
        exit(1);
    }

    // Skip comment lines and find the line with "Nodes: X"
    while (fgets(line, max_buffer_size, file) != NULL)
    {
        // Check if the line was completely read (contains newline)
        if (strchr(line, '\n') == NULL && !feof(file))
        {
            // Line was too long for our buffer, resize and read the rest
            const int new_size = max_buffer_size * 2;
            char* new_line = realloc(line, new_size);
            if (new_line == NULL)
            {
                printf("Memory reallocation failed for line buffer\n");
                free(line);
                fclose(file);
                exit(1);
            }
            line = new_line;
            free(new_line);
            max_buffer_size = new_size;

            // Clear the remainder of the current line
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF);
            continue;
        }

        if (line[0] == '#')
        {
            // Check if this comment line contains the number of nodes
            if (strstr(line, "Nodes:") != NULL)
            {
                sscanf(line, "# Nodes: %d", N);
            }
        }
        else
        {
            // First non-comment line, break
            break;
        }
    }

    // If N wasn't found or is invalid, exit
    if (*N <= 0)
    {
        printf("Error: Could not determine the number of nodes\n");
        free(line);
        fclose(file);
        exit(1);
    }

    // Rewind the file to start reading the edges
    rewind(file);

    // Allocate memory for the hyperlink matrix
    *hyperlink_matrix = (double**) malloc(*N * sizeof(double*));
    if (*hyperlink_matrix == NULL)
    {
        printf("Memory allocation failed for hyperlink matrix rows\n");
        free(line);
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < *N; i++)
    {
        (*hyperlink_matrix)[i] = (double*) calloc(*N, sizeof(double));
        if ((*hyperlink_matrix)[i] == NULL)
        {
            printf("Memory allocation failed for hyperlink matrix column %d\n", i);
            // Free previously allocated memory
            for (int j = 0; j < i; j++)
            {
                free((*hyperlink_matrix)[j]);
            }
            free(*hyperlink_matrix);
            free(line);
            fclose(file);
            exit(1);
        }
    }

    // Read the edges and fill the matrix
    int from_node, to_node;
    while (fgets(line, max_buffer_size, file) != NULL)
    {
        // Handle lines that are too long
        if (strchr(line, '\n') == NULL && !feof(file))
        {
            // Skip the rest of this line
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF);
            continue;
        }

        // Skip comment lines
        if (line[0] == '#') continue;

        // Parse the edge: FromNodeId ToNodeId
        if (sscanf(line, "%d %d", &from_node, &to_node) == 2)
        {
            // Make sure nodes are within range
            if (from_node >= 0 && from_node < *N && to_node >= 0 && to_node < *N)
            {
                // Set the link in the matrix to 1.0 initially
                (*hyperlink_matrix)[to_node][from_node] = 1.0;
            }
            else
            {
                printf("Warning: Node IDs out of range: %d -> %d\n", from_node, to_node);
            }
        }
    }

    // Normalize the matrix columns
    for (int j = 0; j < *N; j++)
    {
        double col_sum = 0.0;

        // Count incoming links
        for (int i = 0; i < *N; i++)
        {
            if ((*hyperlink_matrix)[i][j] > 0)
            {
                col_sum += 1.0;
            }
        }

        // Normalize if there are incoming links
        if (col_sum > 0)
        {
            for (int i = 0; i < *N; i++)
            {
                if ((*hyperlink_matrix)[i][j] > 0)
                {
                    (*hyperlink_matrix)[i][j] = 1.0 / col_sum;
                }
            }
        }
    }

    free(line);
    fclose(file);
}

void read_graph_from_file2(char* filename, int* N, int** row_ptr, int** col_idx, double** val)
{
}

void PageRank_iterations1(const int N, double** hyperlink_matrix, double d, double epsilon, double* scores)
{
    if (scores == NULL) return;

    for (int i = 0; i < N; ++i)
    {
        scores[i] = 1.0 / N;
    }

    char dangling_flag = 0;
    char* dangling_indexes = calloc(N, sizeof(char));
    for (int i = 0; i < N; ++i)
    {
        double col_sum = 0.0;
        for (int j = 0; j < N; ++j)
        {
            col_sum += hyperlink_matrix[j][i];
        }
        if (col_sum == 0.0)
        {
            dangling_indexes[i] = 1;
            if (!dangling_flag) dangling_flag = 1;
        }
    }

    double diff = 100.0;
    double* old_scores = malloc(N * sizeof(double));

    while (diff > epsilon)
    {
        // Calculate sum of scores for dangling pages
        double dangling_scores = 0.0;
        if (dangling_flag)
        {
            for (int i = 0; i < N; ++i)
            {
                if (dangling_indexes[i] == 1)
                {
                    dangling_scores += scores[i];
                }
            }
        }

        // Copy of old scores for diff
        memcpy(old_scores, scores, N * sizeof(double));

        // Zero out scores for the new iteration
        for (int i = 0; i < N; ++i)
        {
            scores[i] = (1 - d + d * dangling_scores) / N;
        }

        // Contribution from incoming edges
        for (int j = 0; j < N; ++j)
        {
            if (dangling_indexes[j] == 0) // Skip dangling nodes
            {
                for (int i = 0; i < N; ++i)
                {
                    if (hyperlink_matrix[i][j] > 0)
                    {
                        scores[i] += d * hyperlink_matrix[i][j] * old_scores[j];
                    }
                }
            }
        }

        // Calculate diff for convergence check
        diff = 0.0;
        for (int i = 0; i < N; ++i)
        {
            diff += fabs(scores[i] - old_scores[i]);
        }
    }

    free(dangling_indexes);
    free(old_scores);
}

void PageRank_iterations2(int N, int* row_ptr, int* col_idx, double* val, double d, double epsilon, double* scores)
{
}

// Descending order
int comp(const void* a, const void* b)
{
    const double diff = **(double**) b - **(double**) a;
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}

void top_n_webpages(int N, double* scores, int n)
{
    if (scores == NULL) return;

    // Sort array of pointers to scores, to easily be able to retrieve original index
    double** score_pointers = malloc(N * sizeof(double*));
    for (int i = 0; i < N; ++i)
    {
        score_pointers[i] = &scores[i];
    }
    qsort(score_pointers, N, sizeof(double), comp);

    printf("Top %d pages, sorted according to PageRank score:\n", n);
    for (int i = 0; i < n; ++i)
    {
        printf("Score [%lf] Index [%ld]\n", *score_pointers[i], score_pointers[i] - scores);
    }

    free(score_pointers);
}
