/**
 * bmp.h
 *
 * Computer Science 50
 * Problem Set 4
 *
 * BMP-related data types based on Microsoft's own.
 */

#include <stdint.h>

/**
 * Common Data Types 
 *
 * The data types in this section are essentially aliases for C/C++ 
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;  // unsigned 8 bit integer (0 to 255), a byte
typedef uint32_t DWORD; // unsigned 32 bit integer (0 to 4294967295 )
typedef int32_t  LONG;  // signed 32 bit integer(–2147483648 to 2147483647 )
typedef uint16_t WORD;  // unsigned 16 bit integer(0 to 65535)

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
typedef struct 
{ 
    WORD   bfType;       //
    DWORD  bfSize;       
    WORD   bfReserved1; 
    WORD   bfReserved2; 
    DWORD  bfOffBits; 
} __attribute__((__packed__)) 
BITMAPFILEHEADER; 

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the 
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
typedef struct
{
    DWORD  biSize;  //4
    LONG   biWidth; //4
    LONG   biHeight; 
    WORD   biPlanes; //2
    WORD   biBitCount; 
    DWORD  biCompression; 
    DWORD  biSizeImage; 
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
} __attribute__((__packed__))
BITMAPINFOHEADER; 

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
