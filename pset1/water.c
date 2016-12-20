#include <cs50.h>
#include <stdio.h>

int main(void)
{
    float gallon_min = 1.5;                 //gallons per minute = 1.5 min
    int gallon_ounces = 128;              //ounces in a gallon = 128 ounces
    
    printf("minutes: ");
    int mins = GetInt();
    int bottles = (mins * gallon_ounces * gallon_min)/16;
    
    printf("bottles : %i\n",bottles);
    
    
    return 0;
}