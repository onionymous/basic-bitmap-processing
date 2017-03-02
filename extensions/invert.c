/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Implementation of the invert filter extension.
Inverts the colours of all the pixels in the image.

Command-line usage: -i
*/

#include "../data/bitmap.h"
#include "invert.h"

void invert_filter(Bitmap *bitmap) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Invert all the bytes
            pixel->b = ~(pixel->b);
            pixel->g = ~(pixel->g);
            pixel->r = ~(pixel->r);
        }
    }
}