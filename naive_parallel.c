#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void read_strings_from_file(const char *file_path, char ***strings, int *num_strings)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int size = 1024; // Initial size of each string buffer
    *num_strings = 0;
    *strings = (char **)malloc(sizeof(char *) * size);

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (*num_strings >= size)
        {
            size *= 2;
            *strings = (char **)realloc(*strings, sizeof(char *) * size);
        }
        (*strings)[*num_strings] = strdup(line);
        (*num_strings)++;
    }

    fclose(file);
}

void naive_string_matching(const char *pattern, const char *text, int text_index)
{
    int pattern_len = strlen(pattern);
    int text_len = strlen(text);
    for (int i = 0; i <= text_len - pattern_len; i++)
    {
        if (strncmp(pattern, &text[i], pattern_len) == 0)
        {
            // Print the correct string index and pattern index
            printf("Pattern found in string %d at index %d in string: %s", text_index, i, text);
        }
    }
}

int main()
{
    char file_path[] = "gene.txt"; // Replace with the path to your input file
    char *pattern = NULL;
    char **strings;
    int num_strings;

    printf("Enter the pattern to search: ");
    size_t len = 0;
    getline(&pattern, &len, stdin);
    pattern[strcspn(pattern, "\n")] = 0; // Remove newline character from the pattern

    read_strings_from_file(file_path, &strings, &num_strings);

// Parallelize the string matching using OpenMP
#pragma omp parallel for
    for (int i = 0; i < num_strings; i++)
    {
        naive_string_matching(pattern, strings[i], i);
        free(strings[i]); // Free allocated memory for individual strings
    }

    free(pattern); // Free allocated memory for the pattern
    free(strings); // Free allocated memory for the array of strings

    return 0;
}
