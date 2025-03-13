#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declarations.h"

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    char* example_graph = "../../Graphs/simple-webgraph.txt";
    char* small_graph = "../../Graphs/100nodes_graph.txt";
    char* large_graph = "../../Graphs/web-stanford.txt";

    int nr_pages1;
    double** graph1;
    read_graph_from_file1(example_graph, &nr_pages1, &graph1);
    printf("Number of pages: %d \n", nr_pages1);
    for (int i = 0; i < nr_pages1; i++)
    {
        for (int j = 0; j < nr_pages1; j++)
        {
            printf(" %lf", graph1[i][j]);
        }
        printf("\n");
    }

    int nr_pages2;
    int* rows;
    int* cols;
    double* vals;
    read_graph_from_file2(example_graph, &nr_pages2, &rows, &cols, &vals);
    // printf("Number of pages: %d \n", pages1);
    // for (int i = 0; i < pages1; i++)
    // {
    //     for (int j = 0; j < pages1; j++)
    //     {
    //         printf(" %lf", graph1[i][j]);
    //     }
    //     printf("\n");
    // }

    const int error = omp_pagerank(argc, argv);
    if (error != 0)
    {
        printf("OMP PageRank failed\n");
    }

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

void PageRank_iterations1(int N, double*** hyperlink_matrix, double d, double epsilon, double* scores)
{
}

void PageRank_iterations2(int N, int* row_ptr, int* col_idx, double* val, double d, double epsilon, double* scores)
{
}

void top_n_webpages(int N, double* scores, int n)
{
}
