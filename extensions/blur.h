/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Header file for blur.c, the blur extension.
*/

#ifndef BLUR_H
#define BLUR_H

//void blur_filter(Bitmap *bitmap, int radius);
void horizontal_blur(Bitmap *bitmap, int radius);
void vertical_blur(Bitmap *bitmap, int radius);

void set_pixel(byte *pixel_array, int width, int row, int col, byte b, byte g, byte r);

#endif
