/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
 
int findMidpoint(int min, int max)
{
    return (max+min)/2;
}
 
// Using binary search
bool search(int value, int values[], int n)   
{
    int min=0;
    int max= n-1;
    if (max < min)          //if list is empty 
        return false;
        
    int midpoint = findMidpoint(min,max);

    for(int i=0;i<n;i++)
    {
        if (values[midpoint] < value)
        {
            min=midpoint+1;
            midpoint = findMidpoint(min,max);
        }
        else if (values[midpoint] > value)
        {   
            max=midpoint-1;
            midpoint = findMidpoint(min, max);
        }
        else
            return true;    //if found return true
    }
    
    return false;           //if it wasn't found return false
}

/**
 * Sorts array of n values.
 */
 
// Using bubble sort
void sort(int values[], int n)   
{
    int counter, temp;
    
    while(counter != 0)       //When counter is , it means that the loop is sorted, no need to iterate again
    {
        counter = 0;
        for (int i=1; i<n; i++)
        {
            if(values[i] < values[i-1])
            {
                temp = values[i];
                values[i] = values[i-1];
                values[i-1] = temp;
                counter++;
            }
        }
    }
    return;
}