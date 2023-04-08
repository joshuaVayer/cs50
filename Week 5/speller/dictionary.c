// Implements a dictionary's functionality
// 1 - IMPORTS
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 140000;

// Hash table
node *table[N];
int dicL = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word and set a new node
    int hashedW = hash(word);
    node *n = table[hashedW];

    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *input)
{
    long count = 0;
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        count += toupper(input[i]);
    }
    return count % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Try to open the file
    FILE *dicP = fopen(dictionary, "r");

    // If the file can't be opened return false
    if (dictionary == NULL)
    {
        printf("Sorry, the file can't be opened \n");
        return false;
    }

    // If the file is opened
    char nextW[LENGTH + 1];
    // Read each string of the file
    while (fscanf(dicP, "%s", nextW) != EOF)
    {
        // Create a node, copy the word and hash it
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, nextW);
        int hashedW = hash(nextW);


        n->next = table[hashedW];
        table[hashedW] = n;

        // Increments the dictionary size
        dicL++;
    }
    // Close file
    fclose(dicP);

    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (dicL >= 0)
    {
        return dicL; 
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Assign cursor
        node *n = table[i];
        while (n != NULL)
        {
            
            node *tmp = n;
            n = n->next;
            free(tmp);
        }
        if (i == (N - 1) && n == NULL)
        {
            return true;
        }
    }
    return false;
}
