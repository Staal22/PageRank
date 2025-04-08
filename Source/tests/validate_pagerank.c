#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Expected results from the test case
const double expected_scores[] = {0.295000, 0.202500, 0.180000, 0.097500, 0.067500, 0.067500, 0.060000, 0.030000};
const int expected_indices[] = {7, 5, 6, 4, 1, 3, 0, 2};
const int NUM_EXPECTED = 8;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <output_file>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Failed to open output file %s\n", argv[1]);
        return 1;
    }

    char line[256];
    int line_count = 0;
    int found_markers = 0;
    int score_count = 0;
    int all_correct = 1;

    while (fgets(line, sizeof(line), fp))
    {
        line_count++;

        // Look for the marker line
        if (strstr(line, "Top 8 pages, sorted according to PageRank score:"))
        {
            found_markers++;
            score_count = 0;
            continue;
        }

        // Parse score lines after the marker
        if (found_markers > 0 && score_count < NUM_EXPECTED)
        {
            double score;
            int index;

            if (sscanf(line, "Score [%lf] Index [%d]", &score, &index) == 2)
            {
                // Check if this score and index match the expected values
                if (abs(score - expected_scores[score_count]) > 0.000001 ||
                    index != expected_indices[score_count])
                {
                    printf("Mismatch at position %d: Expected Score [%f] Index [%d], Got Score [%f] Index [%d]\n",
                           score_count, expected_scores[score_count], expected_indices[score_count], score, index);
                    all_correct = 0;
                }
                score_count++;
            }

            // If we've checked all expected scores in this section, reset for the next section
            if (score_count == NUM_EXPECTED)
            {
                found_markers = 0;
            }
        }
    }

    fclose(fp);

    if (all_correct)
    {
        printf("All PageRank scores and indices match expected values!\n");
        return 0;
    }

    printf("Some PageRank scores or indices do not match expected values.\n");
    return 1;
}
