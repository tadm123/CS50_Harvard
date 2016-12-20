#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    
    float money;
    int money_coins;
    int count=0;
   
   do
   {
    printf("How much change is owed?\n");
    money = GetFloat();
   }while(money < 0);
  
    money_coins = round(money*100);
   
    while(money_coins>=25)      
    {
        money_coins -= 25; 
        count++;
    }
    while(money_coins>=10)
    {   
        money_coins -= 10;
        count++;
    }
    while(money_coins>=5)
    {
        money_coins -= 5;
        count++;
    }
        
    while(money_coins>=1)
    {
        money_coins -= 1;
        count++;
    }
       
    printf("%d\n",count);
        
    
    return 0;
}
