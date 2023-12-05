#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void search(char *text, char *pattern)
{
    int patternLength = strlen(pattern);
    int textLength = strlen(text);

    int badchar[NO_OF_CHARS];

    badCharHeuristic(pattern, patternLength, badchar);

    int s = 0;
    while (s <= (textLength - patternLength))
    {
        int j = patternLength - 1;

        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0)
        {
            printf("Pattern found at index %d\n", s);
            s += (s + patternLength < textLength) ? patternLength - badchar[text[s + patternLength]] : 1;
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

    search(text, pattern);

    free(text);

    return 0;
}
