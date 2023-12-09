#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    struct TrieNode *failureLink;
    int isEndOfWord;
    int patternIndex;
} TrieNode;

// Queue node structure for BFS during construction of failure links
typedef struct QueueNode
{
    TrieNode *node;
    struct QueueNode *next;
} QueueNode;

// Queue structure for BFS
typedef struct Queue
{
    QueueNode *front, *rear;
} Queue;

// Function to initialize a new TrieNode
TrieNode *createNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }

    node->failureLink = NULL;
    node->isEndOfWord = 0;
    node->patternIndex = -1;

    return node;
}

// Function to insert a pattern into the Trie
void insertPattern(TrieNode *root, char *pattern, int patternIndex)
{
    TrieNode *node = root;

    for (int i = 0; pattern[i] != '\0'; i++)
    {
        int index = pattern[i] - 'a';
        if (!node->children[index])
        {
            node->children[index] = createNode();
        }
        node = node->children[index];
    }

    node->isEndOfWord = 1;
    node->patternIndex = patternIndex;
}

// Function to perform BFS to construct failure links
void constructFailureLinks(TrieNode *root)
{
    Queue queue;
    queue.front = queue.rear = NULL;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            root->children[i]->failureLink = root;
            enqueue(&queue, root->children[i]);
        }
    }

    while (!isEmpty(&queue))
    {
        TrieNode *current = dequeue(&queue);

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            TrieNode *child = current->children[i];
            if (child != NULL)
            {
                TrieNode *failureLink = current->failureLink;

                while (failureLink != NULL && failureLink->children[i] == NULL)
                {
                    failureLink = failureLink->failureLink;
                }

                if (failureLink != NULL)
                {
                    child->failureLink = failureLink->children[i];
                }
                else
                {
                    child->failureLink = root;
                }

                enqueue(&queue, child);
            }
        }
    }
}

// Function to perform pattern matching using Aho-Corasick algorithm
void ahoCorasickSearch(TrieNode *root, char *text)
{
    TrieNode *currentState = root;

    for (int i = 0; text[i] != '\0'; i++)
    {
        int index = text[i] - 'a';

        while (currentState != root && currentState->children[index] == NULL)
        {
            currentState = currentState->failureLink;
        }

        if (currentState->children[index] != NULL)
        {
            currentState = currentState->children[index];

            if (currentState->isEndOfWord)
            {
                printf("Pattern found at index %d: Pattern %d\n", i - strlen(text) + 1, currentState->patternIndex);
            }
        }
    }
}

// Function to initialize a new Queue
Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    queue->front = queue->rear = NULL;
    return queue;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue)
{
    return (queue->front == NULL);
}

// Function to enqueue a TrieNode in the queue
void enqueue(Queue *queue, TrieNode *node)
{
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    newNode->node = node;
    newNode->next = NULL;

    if (isEmpty(queue))
    {
        queue->front = queue->rear = newNode;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue a TrieNode from the queue
TrieNode *dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        return NULL;
    }

    QueueNode *temp = queue->front;
    TrieNode *node = temp->node;

    queue->front = temp->next;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);

    return node;
}

// Function to free memory allocated for the Trie
void freeTrie(TrieNode *node)
{
    if (node != NULL)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            freeTrie(node->children[i]);
        }
        free(node);
    }
}

int main()
{
    TrieNode *root = createNode();

    // Example: Inserting patterns "he", "she", "his", "hers"
    insertPattern(root, "he", 0);
    insertPattern(root, "she", 1);
    insertPattern(root, "his", 2);
    insertPattern(root, "hers", 3);

    // Constructing failure links
    constructFailureLinks(root);

    char text[] = "ahishers";
    ahoCorasickSearch(root, text);

    // Freeing memory
    freeTrie(root);

    return 0;
}
