#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{

    string name = GetString();
    
    if (strlen(name) == 0)                           // if string has no characters
        return -1;                                   // returns unsuccesfully
    
    printf("%c", toupper(name[0]));
    
    for(int i=1,n=strlen(name); i<n; i++)           //strlen() will need to be executed just once.
        if (name[i] == ' ')                         // if space
            printf("%c",toupper(name[i+1]));        // print the next character
    
    printf("\n");
    
    return 0; 
}
