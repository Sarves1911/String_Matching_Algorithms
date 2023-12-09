#include <stdio.h>
#include <string.h>

#define d 256 // Number of characters in the input alphabet
#define q 101 // A prime number for the hash function

// Function to implement the Rabin-Karp algorithm
void searchRabinKarp(char *pattern, char *text)
{
    int M = strlen(pattern);
    int N = strlen(text);
    int i, j;
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for current window of text
    int h = 1;

    // Calculate h, the hash multiplier
    for (i = 0; i < M - 1; i++)
    {
        h = (h * d) % q;
    }

    // Calculate the hash value of the pattern and the first window of text
    for (i = 0; i < M; i++)
    {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // Slide the pattern over the text one character at a time
    for (i = 0; i <= N - M; i++)
    {
        // Check the hash values of the current window of text and the pattern
        if (p == t)
        {
            // If the hash values match, check each character in the pattern
            for (j = 0; j < M; j++)
            {
                if (text[i + j] != pattern[j])
                    break;
            }

            // If pattern matches the window, print the index
            if (j == M)
            {
                printf("Pattern found at index %d\n", i);
            }
        }

        // Calculate the hash value for the next window of text
        if (i < N - M)
        {
            t = (d * (t - text[i] * h) + text[i + M]) % q;

            // Ensure the hash value is non-negative
            if (t < 0)
            {
                t = (t + q);
            }
        }
    }
}

int main()
{
    char text[] = "AABAACAADAABAABA";
    char pattern[] = "AABA";

    printf("Text: %s\n", text);
    printf("Pattern: %s\n", pattern);

    searchRabinKarp(pattern, text);

    return 0;
}
