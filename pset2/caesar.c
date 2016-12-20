#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])       //usage: caesar.c 13
{   
    if(argc != 2)
    {
        printf("Error! usage: caesar.c num");
        return 1;
    }
       
    int k = atoi(argv[1]);
  

    string p = GetString();
    
    
    for(int i=0,n=strlen(p); i<n; i++)
    {
        if (isupper(p[i]))
           printf("%c", (((p[i]-'A')+k)%26 +'A'));    
           
        else if(islower(p[i]))
           printf("%c", (((p[i]-'a')+k)%26 + 'a'));
           
        else
           printf("%c",p[i]);
    }
    printf("\n");
    
    return 0;
}