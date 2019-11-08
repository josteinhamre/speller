// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Counter to keep track of words
int size_counter = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    // Defines temp, the header of below code.
    node *temp = root;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Reassigns root to temp at the begining of every word.
        temp = root;
        // Increments a counter for every word
        size_counter++;
        // Iterate over the characters in the word
        for (int i = 0;  word[i] != 0x00; ++i)
        {
            // Creates an idex based on the character
            int index = (word[i] == 39) ? 26 : word[i] - 'a';
            // checks if the character exists, and reassigns temp one level down if it does.
            if (temp->children[index])
            {
                temp = temp->children[index];
            }
            // If it does not exist, it gets created and assigned to temp
            else
            {
                temp->children[index] = malloc(sizeof(node));
                temp = temp->children[index];
                // Every character one level down is initialized with null
                for (int j = 0; j < N; ++j)
                {
                    temp->children[j] = NULL;
                    temp->is_word = NULL;
                }
            }
            // Validates word if it is the last character of the word.
            if (word[i + 1] == 0x00)
            {
                temp->is_word = true;
            }
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return size_counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Reassignes root to temp
    node *temp = root;

    // Iterates over the characters in the word
    for (int i = 0; word[i] != 0x00; ++i)
    {
        // Creates an idex based on the character, removes capitalization.
        int index = (word[i] < 95) ? word[i] - 'A' : word[i] - 'a';

        // Checks if character is ', assigns special value if true.
        if (word[i] == 39)
        {
            index = 26;
        }

        // Checks if character exist
        if (temp->children[index])
        {
            // Reassigns temp one level down if it does
            temp = temp->children[index];
            // If it is the end of the word, and it is valid, returns true.
            if (word[i + 1] == 0x00 && temp->is_word)
            {
                return true;
            }
        }
        // Returns false if character does not exist.
        else
        {
            return false;
        }
    }
    return false;
}

bool unload_recurr(node *unload_temp);

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Uses reccursion to free all mallocs
    bool unloaded = unload_recurr(root);
    return unloaded;
}
// Recursive unload function.
bool unload_recurr(node *unload_temp)
{
    // Iterates over characters
    for (int i = 0; i < N; ++i)
    {
        // If found, calls the function on the new character node
        if (unload_temp->children[i])
        {
            unload_recurr(unload_temp->children[i]);
        }
    }

    // When no more characters are found, frees the current caracter node
    free(unload_temp);
    return true;
}




