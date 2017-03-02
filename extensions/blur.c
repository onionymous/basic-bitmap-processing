/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Implementation of box blur with repeating edges.
*/

#include <stdlib.h>

#include "../data/bitmap.h"
#include "blur.h" 

void horizontal_blur(Bitmap *bitmap, int radius) {
    // Get the bitmap dimensions
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    // Create a new pixel array
    byte *blurred_array = malloc(width * height * 3);

    // Loop through the rows with a sliding window for the blur
    for (int row = 0; row < height; row++) {
        // Generate sliding window for the first pixel
        int sum_b = 0;
        int sum_g = 0;
        int sum_r = 0;
        int count = 0;

        for (int i = -radius; i <= radius; i++) {
            Pixel *pixel = get_pixel(bitmap, row, i);
            sum_b += pixel->b;
            sum_g += pixel->g;
            sum_r += pixel->r;
            count++;
        }

        byte avg_b = (byte) (sum_b / count);
        byte avg_g = (byte) (sum_g / count);
        byte avg_r = (byte) (sum_r / count);

        set_pixel(blurred_array, width, row, 0, avg_b, avg_g, avg_r);

        // Shift the sliding window by one position for the rest
        for (int col = 1; col < width; col++) {
            // Subtract the pixel at the back
            Pixel *pixel = get_pixel(bitmap, row, col-radius-1);
            sum_b -= pixel->b;
            sum_g -= pixel->g;
            sum_r -= pixel->r;

            // Add one pixel at the front
            pixel = get_pixel(bitmap, row, col+radius);
            sum_b += pixel->b;
            sum_g += pixel->g;
            sum_r += pixel->r;

            // Get the average of the values and set the pixel to that value
            avg_b = (byte) (sum_b / count);
            avg_g = (byte) (sum_g / count);
            avg_r = (byte) (sum_r / count);

            set_pixel(blurred_array, width, row, col, avg_b, avg_g, avg_r);
        }
    }

    // Free the existing pixel array
    free(bitmap->pixel_array);

    // Set the bitmap's pixel array to the new blurred array
    bitmap->pixel_array = blurred_array;
}

void vertical_blur(Bitmap *bitmap, int radius) {
    // Get the bitmap dimensions
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    // Create a new pixel array
    byte *blurred_array = malloc(width * height * 3);

    // Loop through the columns with a sliding window for the blur
    for (int col = 0; col < width; col++) {
        // // Generate sliding window for the first pixel
        int sum_b = 0;
        int sum_g = 0;
        int sum_r = 0;
        int count = 0;

        for (int i = -radius; i <= radius; i++) {
            Pixel *pixel = get_pixel(bitmap, i, col);
            sum_b += pixel->b;
            sum_g += pixel->g;
            sum_r += pixel->r;
            count++;
        }

        byte avg_b = (byte) (sum_b / count);
        byte avg_g = (byte) (sum_g / count);
        byte avg_r = (byte) (sum_r / count);

        set_pixel(blurred_array, width, 0, col, avg_b, avg_g, avg_r);

        // // Shift the sliding window by one position for the rest
        for (int row = 1; row < height; row++) {
            // Subtract the pixel at the back
            Pixel *pixel = get_pixel(bitmap, row-radius-1, col);
            sum_b -= pixel->b;
            sum_g -= pixel->g;
            sum_r -= pixel->r;

            // Add one pixel at the front
            pixel = get_pixel(bitmap, row+radius, col);
            sum_b += pixel->b;
            sum_g += pixel->g;
            sum_r += pixel->r;

            // Get the average of the values and set the pixel to that value
            avg_b = (byte) (sum_b / count);
            avg_g = (byte) (sum_g / count);
            avg_r = (byte) (sum_r / count);

            set_pixel(blurred_array, width, row, col, avg_b, avg_g, avg_r);
        }
    }

    // Free the existing pixel array
    free(bitmap->pixel_array);

    // Set the bitmap's pixel array to the new blurred array
    bitmap->pixel_array = blurred_array;
}

// void blur_filter(Bitmap *bitmap, int radius) {
//     horizontal_blur(bitmap, radius);
//     vertical_blur(bitmap, radius);
// }

void set_pixel(byte *pixel_array, int width, int row, int col, byte b, byte g, byte r) {
    pixel_array[row*(width*3) + (col*3+B)] = b;
    pixel_array[row*(width*3) + (col*3+G)] = g;
    pixel_array[row*(width*3) + (col*3+R)] = r;
}