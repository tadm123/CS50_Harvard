#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, string argv[])   
{
     if(argc != 2)
    {
        printf("Error! usage: caesar.c num\n");
        return 1;
    }
   
    string word = argv[1];            // to make it simpler
    
    for(int i=0; i<strlen(word); i++)
        if(!isalpha(word[i]))
        {
            printf("Please enter a all alphabetical letters\n");
            return 1;
        }
        
   
    string p = GetString();
    
    for(int i=0,j=0,n=strlen(p); i<n; i++)
    {
        if (isupper(p[i]))
        {
            printf("%c", (( (p[i]-'A') + (toupper(word[j])-'A')  )%26 +'A'));    
            j++;
        }
        
        else if(islower(p[i]))
        {
            printf("%c", (( (p[i]-'a') + (tolower(word[j])-'a')  )%26 + 'a'));
            j++;
        }
        else
           printf("%c",p[i]);
         
        if (word[j] == '\0')             // if character is NULL, go to the beginning
            j=0;
        
    }
    printf("\n");
    
    return 0;
}