/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

monochrome.c - Implements various color filter functions - greyscale, RGB filters, etc.
*/

#include "../data/bitmap.h"
#include "monochrome.h"

// Greyscale implementation based on the ITU-R 601-2 luma transform
// Weights RGB components differently to account for differences in human
// color perception
void greyscale_filter(Bitmap *bitmap) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Get the pixel at the pixel array
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Calculate the weighted greyscale average
            float greyscale = LUMA_B * pixel->b + LUMA_G * pixel->g + LUMA_R * pixel->r;

            // Set all the pixels to the weighted greyscale average
            pixel->b = (unsigned char) greyscale;
            pixel->g = (unsigned char) greyscale;
            pixel->r = (unsigned char) greyscale;
        }
    }
}

// Filter that isolates the red channel
void red_filter(Bitmap *bitmap) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Get the pixel at the pixel array
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Isolate red channel by setting non-red channels to zero
            pixel->b = 0x00;
            pixel->g = 0x00;
        }
    }
}

// Filter that isolates the blue channel
void blue_filter(Bitmap *bitmap) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Get the pixel at the pixel array
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Isolate red channel by setting non-red channels to zero
            pixel->g = 0x00;
            pixel->r = 0x00;
        }
    }
}

// Filter that isolates the green channel
void green_filter(Bitmap *bitmap) {
    int width = bitmap->info_header->width;
    int height = bitmap->info_header->height;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Get the pixel at the pixel array
            Pixel *pixel = get_pixel(bitmap, row, col);

            // Isolate red channel by setting non-red channels to zero
            pixel->b = 0x00;
            pixel->r = 0x00;
        }
    }
}