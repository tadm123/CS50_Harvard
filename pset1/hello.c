#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("hello, world\n");
    string s;
    s = GetString();
    
    for(int i=0;i<2;i++)
        printf("%s\n",s);
    
    return 0;
}