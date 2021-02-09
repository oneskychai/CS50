// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 143074;

// Hash table
node *table[N];

// Create boolean to track load success
bool load_success;

// Dictionary word count
unsigned int dict_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Create buffer to store each word coming in
    char buffer[LENGTH + 1];
    strcpy(buffer, word);

    // Convert buffer to lowercase
    for (int i = 0; (buffer[i]); i++)
    {
        if (buffer[i] < 91 && buffer[i] > 64)
        {
            buffer[i] = buffer[i] + 32;
        }
    }

    // Obtain hash value of lowercase word
    unsigned int hash_lower = hash(buffer);

    // Check table[hash_lower] for word
    // Create pointer trav to traverse linked list
    node *trav = table[hash_lower];
    while (trav != NULL)
    {
        // If word found return true
        if (strcmp(trav->word, buffer) == 0)
        {
            return true;
        }

        // Move to next node of list
        trav = trav->next;
    }

    // If list traversed to end return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Adapting and modifying djb2 hash function found at http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 2478;
    int c;

    while ((c = *word++))
    {
        hash = hash * 23 + c;
    }

    // Take hash mod N to make sure it will fit into the hash table
    hash = hash % N;

    // Cast hash to unsigned int hash_out
    unsigned int hash_out = (unsigned int)hash;

    return hash_out;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open file for dictionary
    FILE *dict_ptr = fopen(dictionary, "r");
    if (dict_ptr == NULL)
    {
        fprintf(stderr, "Unable to open %s.\n", dictionary);
        load_success = false;
        return false;
    }

    // Create buffer to store each word coming in
    char buffer[LENGTH + 1];

    // Set pointers in hash table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Create variable to store hash value
    unsigned int hash_val;

    // Read strings in one at a time
    while (fscanf(dict_ptr, "%s", buffer) != EOF)
    {
        // Allocate memory for node, check for NULL
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fprintf(stderr, "Ran out of memory.\n");
            load_success = false;
            return false;
        }

        // Copy word from buffer to node
        strcpy(n->word, buffer);

        // Calculate hash value and insert node into hash table
        hash_val = hash(buffer);
        if (table[hash_val] == NULL)
        {
            table[hash_val] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[hash_val];
            table[hash_val] = n;
        }

        // Add 1 to dict_count
        dict_count++;
    }

    // Close dictionary file
    fclose(dict_ptr);
    load_success = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (!load_success)
    {
        return 0;
    }
    else
    {
        return dict_count;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Loop through buckets in hash table
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
