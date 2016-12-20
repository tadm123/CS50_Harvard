#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n,sp,hs;
    
    do
    {
        printf("Height: ");
        n=GetInt();         
    }
    while(n<0 || n>23);
    
    
    for (int i=0;i<n;i++)       
    {
        for(sp=(n-1)-i;sp>0;sp--)        
            printf(" ");              
        
        for(hs=(n-1)-i; hs<=n;hs++)     
            printf("#");
        printf("\n");
    }
        
    
    return 0;
}



//  ##    i=0, (spaces= 2, hashes=2)     
// ###    i=1, (spaces =1, hashes=3)     
//####    i=2, (spaces =0, hashes=4)

