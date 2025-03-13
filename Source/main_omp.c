#include <stdio.h>
#include <omp.h>
#include "function_declarations.h"

int omp_pagerank(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

#pragma omp parallel default(none)
    {
#pragma omp single
        {
            printf("Total threads: %d\n", omp_get_num_threads());
        }
        printf("Hello World... from thread = %d\n",
               omp_get_thread_num());
    }

    return 0;
}

void PageRank_iterations2_omp(int N, int* row_ptr, int* col_idx, double* val, double d, double epsilon, double* scores)
{
}

void top_n_webpages_omp(int N, double* scores, int n)
{
}
