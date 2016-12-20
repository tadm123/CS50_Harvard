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
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;   //0, 1,2 or 3 padding of zeros

    int red;
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            

                red = triple.rgbtRed;    
                
                // using http://www.rapidtables.com/web/color/gray-color.htm table to scale it into a shade of grey
                if(red > 235 && red <= 255)
                {
                    triple = (RGBTRIPLE) {255,255,255};
                }
                else if(red > 220 && red <= 235)
                {
                    triple = (RGBTRIPLE) {211,211,211};
                }
                else if(red > 210 && red <= 220)
                {
                    triple = (RGBTRIPLE) {192,192,192};
                }
                else if(red > 175 && red < 200)
                {
                    triple = (RGBTRIPLE) {169,169,169};
                }
                else if(red > 150 && red <= 175)
                {
                    triple = (RGBTRIPLE) {128,128,128};
                }
                else if(red > 125 && red <= 150)
                {
                    triple = (RGBTRIPLE) {105,105,105};
                }
                else if(red > 75 && red <= 100)
                {
                    triple = (RGBTRIPLE) {119,136,153};
                }
                else if(red > 50 && red <= 75)
                {
                    triple = (RGBTRIPLE) {112,128,144};
                }
                else if(red > 25 && red <= 50)
                {
                    triple = (RGBTRIPLE) {47,79,79};
                }
                else if(red >= 0 && red <= 25)
                {
                    triple = (RGBTRIPLE) {0,0,0};
                }
                
            

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
