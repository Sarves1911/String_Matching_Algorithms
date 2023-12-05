#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NO_OF_CHARS 256

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void badCharHeuristic(char *pattern, int patternLength, int badchar[NO_OF_CHARS])
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (i = 0; i < patternLength; i++)
        badchar[(int)pattern[i]] = i;
}

void parallelSearch(char *text, char *pattern, int textLength, int patternLength, int badchar[NO_OF_CHARS])
{
#pragma omp parallel for
    for (int s = 0; s <= (textLength - patternLength); s++)
    {
        int j = patternLength - 1;
        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0)
        {
#pragma omp critical
            printf("Pattern found at index %d\n", s);
            s += patternLength;
        }
        else
        {
            s += max(1, j - badchar[text[s + j]]);
        }
    }
}

int main()
{
    FILE *fp = fopen("gene.txt", "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char *text = malloc(fileSize + 1);

    if (text == NULL)
    {
        perror("Memory allocation error");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fread(text, 1, fileSize, fp);
    text[fileSize] = '\0';
    fclose(fp);

    char pattern[100]; // Assuming maximum pattern length is 100 characters
    printf("Enter the pattern to search: ");
    scanf("%s", pattern);

    int patternLength = strlen(pattern);
    int badchar[NO_OF_CHARS];
    badCharHeuristic(pattern, patternLength, badchar);

    parallelSearch(text, pattern, fileSize, patternLength, badchar);

    free(text);

    return 0;
}
