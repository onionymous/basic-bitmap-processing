/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Header file for structs.c, contains structs for the project
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#define BMPHEADER_SIZE 54
#define BYTES_IN_PIXEL 3

// Tell the compiler not to add alignment/padding bytes to the structs in memory
// Necessary to make the struct equivalent to the BMP header structure

#pragma pack(push, 1)

typedef struct {
    // 14 bytes
    unsigned char header1;                  // B
    unsigned char header2;                  // M
    unsigned int bmp_size;                  // size of BMP file in bytes
    unsigned short reserved1;               // 2 bytes reserved value (unused)
    unsigned short reserved2;
    unsigned int pixel_array_offset;        // starting address of pixel array
} BitmapFileHeader;

typedef struct {
    // 40 bytes
    // Based on the Windows BITMAPINFOHEADER standard
    unsigned int infoheader_size;           // size of BITMAPINFOHEADER
    int          width;                     // bitmap width in pixels (signed)
    int          height;                    // bitmap height in pixels (signed)
    unsigned short color_planes;            // must be 1
    unsigned short bits_per_pixel;          // typically 1,4,8,16,24,32
    unsigned int compression_method;
    unsigned int image_size;                // size of raw bitmap data
    unsigned int h_res;                     // horizontal resolution
    unsigned int v_res;                     // vertical resolution
    unsigned int color_palette;             // number of colors in palette
    unsigned int important_colors;          // default 0
} BitmapInfoHeader;

#pragma pack(pop)

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Pixel;

#endif