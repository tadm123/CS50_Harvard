/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    int n = atoi(argv[1]);
    
    if (n < 1 || n>= 100)
    {
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //old
    int oldWidth = bi.biWidth;   //in pixels
    int oldHeight = abs(bi.biHeight);
    int oldPadding =  (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;  //padding = 3
    
    //new
    bi.biWidth *= n;
    bi.biHeight *= n;

    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  //padding = 3
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) *abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    for (int i = 0;  i < oldHeight; i++)  //oldHeight = 3
    {  
        for(int a=0; a<n;a++)               //resizing vertically
        {
            // iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)  //oldWidth = 3
            {
                
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile (repeated n times for each pixel)
                for(int r=0; r< n ; r++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

                // skip over padding, if any
             fseek(inptr, oldPadding, SEEK_CUR);  //skipping padding of infile
            
        
          // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)   //padding = 3 for small.bmp
            {
                fputc(0x00, outptr);
            }
            
            if(a != n-1)
                fseek(inptr, -(oldWidth*3)-oldPadding, SEEK_CUR);    
        }
   
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
