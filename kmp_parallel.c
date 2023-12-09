#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void computeLPSArray(char *pattern, int M, int *lps)
{
    int len = 0;
    int i = 1;

    lps[0] = 0;

    while (i < M)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void parallelKMPSearch(char *pattern, char *text)
{
    int M = strlen(pattern);
    int N = strlen(text);

    int *lps = (int *)malloc(sizeof(int) * M);
    if (lps == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    computeLPSArray(pattern, M, lps);

    int i, j;
    int pattern_found = 0;

#pragma omp parallel for private(j) shared(pattern_found)
    for (i = 0; i < N; i++)
    {
        if (pattern_found)
            continue;

        j = 0;

        while (j < M && pattern[j] == text[i + j])
        {
            j++;
        }

        if (j == M)
        {
#pragma omp critical
            {
                if (!pattern_found)
                {
                    printf("Pattern found at index %d\n", i);
                    pattern_found = 1;
                }
            }
        }
    }

    free(lps);
}

int main()
{
    FILE *file;
    char *filename = "gene.txt";
    char pattern[] = "AGCT"; // Replace with the pattern you want to search

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *text = (char *)malloc(sizeof(char) * (file_size + 1));
    if (text == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    fread(text, 1, file_size, file);
    text[file_size] = '\0';

    fclose(file);

    parallelKMPSearch(pattern, text);

    free(text);

    return 0;
}
