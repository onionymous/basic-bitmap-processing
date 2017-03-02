/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Implementation of the threshold filter extension.
Turns pixels black if they are below a certain threshold, and white
if they are above.

Command-line usage: -t <threshold>
*/

#include "../data/bitmap.h"
#include "threshold.h"

void threshold_filter(Bitmap *bitmap, float threshold) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Get the pixel at the pixel array
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Calculate the average
            float average = ((float)(pixel->b + pixel->g + pixel->r) / 3) / 255;
            if (average > threshold) {
                // 0xFF is white
                pixel->b = 0xFF;
                pixel->g = 0xFF;
                pixel->r = 0xFF;
            } else {
                // 0x00 is black
                pixel->b = 0x00;
                pixel->g = 0x00;
                pixel->r = 0x00;
            }
        }
    }
}