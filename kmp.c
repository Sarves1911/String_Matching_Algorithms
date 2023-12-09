#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compute the LPS (Longest Proper Prefix which is also Suffix) array
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

// Function to perform pattern searching using KMP algorithm
void KMPSearch(char *pattern, char *text)
{
    int M = strlen(pattern);
    int N = strlen(text);

    // Create lps[] that will hold the Longest Proper Prefix values for pattern
    int *lps = (int *)malloc(sizeof(int) * M);
    if (lps == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Preprocess the pattern
    computeLPSArray(pattern, M, lps);

    int i = 0; // Index for text[]
    int j = 0; // Index for pattern[]

    while (i < N)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            printf("Pattern found at index %d\n", i - j);
            j = lps[j - 1];
        }
        else if (i < N && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
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

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read the contents of the file into a buffer
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
    text[file_size] = '\0'; // Null-terminate the string

    fclose(file);

    // Perform pattern search
    KMPSearch(pattern, text);

    free(text);

    return 0;
}
