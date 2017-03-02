/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Implementation of the crop extension.
Crops a specified number of pixels from each margin.

Command-line usage: -c <top> <bottom> <left> <right>
*/
#include <stdlib.h>

#include "../data/bitmap.h"
#include "crop.h"

void crop_filter(Bitmap *bitmap, int top, int bottom, int left, int right) {
    // TO DO: Implement error checking

    // Get the existing properties of the bitmap
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    // Set the properties of the new bitmap
    int new_width = width - left - right;
    int new_height = height - top - bottom;
    int new_padding = get_padding(new_width);

    // Create a new pixel array
    byte *cropped_array = malloc(new_width * new_height * 3);

    // Copy the values from the old pixel array to the new array
    int r = 0;
    for (int row = bottom; row < height-top; row++) {
        int c = 0;
        for (int col = left; col < (width-right); col++) {
            // Get the existing pixel
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Copy the BGR components to the new pixel array
            cropped_array[r*(new_width*3) + (c+B)] = pixel->b;
            cropped_array[r*(new_width*3) + (c+G)] = pixel->g;
            cropped_array[r*(new_width*3) + (c+R)] = pixel->r;
            c+=3;
        }
        r++;
    }

    // Set the new bitmap properties in the headers
    bitmap->info_header->width = new_width;
    bitmap->info_header->height = new_height;
    bitmap->info_header->image_size = (new_width*3 + new_padding) * new_height;
    bitmap->file_header->bmp_size = (bitmap->info_header->image_size) + BMPHEADER_SIZE;

    // Free the existing pixel array
    free(bitmap->pixel_array);

    // Set the bitmap's pixel array to the new cropped array
    bitmap->pixel_array = cropped_array;

}