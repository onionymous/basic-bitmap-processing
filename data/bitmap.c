/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

bitmap.c, contains implementation of bitmap processing functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bitmap.h"

// Creates a new bitmap file and reads the bitmap header of a given file.
// By default it does not read the pixel array.
Bitmap *new_bitmap(char *filename) {
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    BitmapFileHeader *file_header;
    BitmapInfoHeader *info_header;
    byte *pixel_array = NULL;

    // Reads bitmap file header and DIB header
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        // Error checking for file I/O errors
        printf("Error opening file %s: %d (%s)\n", filename, errno, strerror(errno));
        return NULL;
    } else {
        // Read the 14-byte file header
        file_header = malloc(sizeof(BitmapFileHeader));
        fread(file_header, sizeof(BitmapFileHeader), 1, fp);

        if (file_header->header1 == 'B' && file_header->header2 == 'M') {
            // Check for valid bitmap file
            // Read the 40-byte DIB header
            info_header = malloc(sizeof(BitmapInfoHeader));
            fread(info_header, sizeof(BitmapInfoHeader), 1, fp);
        } else {
            // If not a valid BMP file, quit and exit
            printf("%s is not a valid bmp file.\n", filename);
            return NULL;
        }

        fclose(fp);
    }

    bitmap->file_header = file_header;
    bitmap->info_header = info_header;
    bitmap->pixel_array = pixel_array;

    // Bitmap should be non-NULL if no errors occurred
    return bitmap;
}

// Read the pixel array of a bitmap file
void read_pixels(Bitmap *bitmap, char *filename) {
    
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;
    int padding = get_padding(width);
    unsigned int offset = bitmap->file_header->pixel_array_offset;

    FILE *fp = fopen(filename, "rb");
    
    // Move the file pointer to the start of the pixel array
    fseek(fp, offset, SEEK_SET);

    // Allocate memory for the pixel array (excluding padding bytes)
    byte *pixel_array = malloc(width * height * 3);

    for (int row = 0; row < height; row++) {
        // Read the pixels into a pixel array, one row at a time
        fread(&pixel_array[row*(width*3)], sizeof(byte)*3, width, fp);

        // Read and discard the padding bytes
        byte padding_byte;
        for (int p = 0; p < padding; p++) {
            fread(&padding_byte, sizeof(byte), 1, fp);
        }
    }

    fclose(fp);
    bitmap->pixel_array = pixel_array;
}

// Free the bitmap
void free_bitmap(Bitmap *bitmap) {
    free(bitmap->file_header);
    free(bitmap->info_header);
    free(bitmap->pixel_array);
    free(bitmap);
}

void write_bitmap(Bitmap *bitmap, char *fileout) {
    FILE *fp = fopen(fileout, "wb");

    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    int padding = get_padding(width);

    // Write the bitmap file header and the DIB header
    fwrite(bitmap->file_header, sizeof(BitmapFileHeader), 1, fp);
    fwrite(bitmap->info_header, sizeof(BitmapInfoHeader), 1, fp);

    // Write the pixel array
    byte *pixel_array = bitmap->pixel_array;

    for (int row = 0; row < height; row++) {
        // Write the pixels, one row at a time
        fwrite(&pixel_array[row*(width*3)], sizeof(byte)*3, width, fp);

        // Add extra padding bytes for each row
        for (int p = 0; p < padding; p++) {
            byte null = 0;
            fwrite(&null, sizeof(byte), 1, fp);
        }
    }

    fclose(fp);
}

// Get the number of padding bytes for a specified bitmap width
int get_padding(int width) {
    int padding = 0; //set padding to 0 by default

    // If row size is not a multiple of 4
    if ((width*3) % 4 != 0) {
        // Determine the bytes needed to pad a row to a multiple of 4
        padding = 4 - ((width*3) % 4);
    }

    return padding;
}

// Gets a single pixel from the pixel array of a bitmap, given the
// row and column of the pixel
Pixel *get_pixel(Bitmap *bitmap, int row, int col) {
    Pixel *pixel = NULL;
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    // If the specified row and column is beyond the dimensions of the image
    // Wrap around (i.e. tile a flipped version of the image adjacent to it)
    if (col >= width) {
        col = width - (col % width);
    } else if (col < 0) {
        col = -col;
    }

    if (row >= height) {
        row = height - (row % height);
    } else if (row < 0) {
        row = -row;
    }

    pixel = (Pixel*) &(bitmap->pixel_array[row*(width*3) + (col*3)]);

    return pixel;
}