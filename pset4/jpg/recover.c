/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;       // or unsigned char

bool check(BYTE buffer[]);

int main(void)
{
    
    // open memory card file
    FILE *fp_input = fopen("card.raw", "r");
    if (fp_input == NULL)
    {
        printf("Couldn't open %s.", "card.raw");
        return 1;
    }
    
    
    BYTE buffer[512];                   
    char title[8];                     // ex: title = "002.jpg"
    int counter = -1;
    FILE *fp_output = NULL;
      
      
    while(feof(fp_input) == 0 && fread(buffer, 1, 512, fp_input) == 512)    //feof returns nonzero if end-of-file indicator is set
    {  
 
        if (check(buffer))   //if its a start of a new jpg
        {
            counter++;
            if (counter == 0)
            {
                sprintf(title, "%03d.jpg", counter);     
                fp_output = fopen(title, "a");     //create new file named 001.jpg
            }
            
            else //if counter is not 1  (ex: counter = 2)
            {
                fclose(fp_output);   //close last one (close 001.jpg)
                sprintf(title, "%03d.jpg", counter);    //002.jpg
                fp_output = fopen(title, "a");    //open 002.jpg
                if (fp_output == NULL)
                {
                    fprintf(stderr, "Couldn't open %s.", title);
                    return 1;
                }
                
            }
        }       
            
        if (fp_output != NULL)  //if fp_out is already opened
            fwrite(buffer, 1, 512, fp_output);           //keep writing in it
                                                        
    } 
    
   
    fclose(fp_input);
    fclose(fp_output);
    return 0;
}

bool check(BYTE buffer[])   //checks if first 4 bytes are start of jpeg
{
    if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff    //if jpg is found
                  && (buffer[3] >= 0xe0 && buffer[3] <= 0xef) )
                  return true;              //returns true if start of jpeg
    else
        return false;                       //returns false if not start of jpeg
}