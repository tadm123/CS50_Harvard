/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "dictionary.h"

#define SIZE 26

typedef  struct node
{
    char word[LENGTH+1];
    struct node *next;
}node;

unsigned int hash_num = 0;
node *hashtable[SIZE];  //26 letters in alphabet

node *head = NULL;


unsigned int hash(char const *key)  //input is a string, outputs a number from 0-26
{
    unsigned int hash= tolower(key[0]) - 'a';
    return hash % SIZE;
}


/**
 * Returns true if word is in dictionary else false.
 */
  
bool check(const char* word)    //check all words from the text and compare it with the word on the hashtable(that is in memory)
{
    // converting word to lowercase
    char word_lower[LENGTH+1];
    int i;
    for(i = 0; word[i] != '\0'; i++)
        word_lower[i] = tolower(word[i]);
    
    word_lower[i] = '\0';
    node *temp = hashtable[hash(word_lower)];  //temp points to node in hastable (hashtable[0])
    
    //if(temp == NULL)      
     //   return false;
    //traversing that linked list
    
    for(; temp != NULL; temp = temp->next)
        if( strcmp(temp->word,word_lower) == 0)
            return true;
    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    
    unsigned int hash_index=0;
    
    FILE *fp = fopen(dictionary, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Couldn't open %s",dictionary);
        return false;
    }
    
    //dictionary 
    

    while(!feof(fp))   //breaks when reaches end-of-file
    {
        node *temp = malloc(sizeof(node));
        if (temp == NULL)
        {
            unload();
            fclose(fp);
            return false;
        }
            
            
        if(fscanf(fp , "%s", temp->word) == 1)   //storing word of dictionary in my new_node -> word
        {
            hash_index = hash(temp->word); 
            head = hashtable[hash_index];    //head always points to first element of linked list (containting word of dictionary)
            
            
            temp->next = head;  //preserve
            head = temp;        //preserve
            hashtable[hash_index] = temp;
        
            hash_num++;
        }
        else    //if fscanf couldn't store the word (return 0)
        {
            free(temp);   //free last temp
            break;
        }
    }
    
    
    fclose(fp);
    return true;
   
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return hash_num;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{

    for(int i=0; i<SIZE; i++)
    {
        if(hashtable[i] != NULL)      //if hashtable isn't NULL (has nodes)
        {
            node *cursor = hashtable[i];        //cursor points at head of individual linked list
            while(cursor != NULL)       //free them
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    
    return true;
}
