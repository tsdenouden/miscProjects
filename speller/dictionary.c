// Implements a dictionary's functionality

//Libraries
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
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

//Dictionary file
FILE *dict = NULL;

//26^2 = 676 buckets
//From A to Z and Aa to Zz (26^1 = 26, 26^2 = 676, 26 + 676 = 702)
const unsigned int N = 702;

// Hash table
node *table[N];

//Total amount of words in the dictionary
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Hash word to get linked list index
    int word_index = hash(word);
    //Look through linked list for word
    for (node *cursor = table[word_index]; cursor != NULL; cursor = cursor->next)
    {
        //Compare strings
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int word_pos = 0;
    //26^2 = 676 total indexes
    //Each letter has 26 combinations, each with another letter
    //Final index = (First letter's alphabetical position*26) + Second letter's alphabetical position;
    if (strlen(word) > 1)
    {
        word_pos = ((toupper(word[0]) - 'A') * 26) + (toupper(word[1]) - 'A') + 26;
    }
    else
    {
        word_pos = (toupper(word[0]) - 'A');
    }
    return word_pos;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open dictionary
    dict = fopen(dictionary, "r");
    //Create buffer
    char buffer[LENGTH + 1];
    //Check if dictionary exists
    if (dict == NULL)
    {
        return false;
    }
    //Read entire file, one at a time into buffer
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        //Allocate memory for new word
        node *text = malloc(sizeof(node));
        if (text == NULL)
        {
            return false;
        }
        //Copy word from buffer
        strcpy(text->word, buffer);
        text->next = NULL;
        //Hash
        unsigned int node_index = hash(buffer);
        text->next = table[node_index];
        table[node_index] = text;
        word_count += 1;
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Go through every index
    for (int i = 0; i < N; i++)
    {
        //If not null,
        if (table[i] != NULL)
        {
            //Two pointers set to linked list
            node *cursor = table[i];
            node *tmp = cursor;
            while (cursor != NULL)
            {
                //Temp pointer pointing to cursor
                tmp = cursor;
                //Cursor moves to next index
                cursor = cursor->next;
                //Free previous index
                free(tmp);
            }
        }
    }
    //Close file
    fclose(dict);
    return true;
}
