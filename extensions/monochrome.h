/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Header file for monochrome.c, includes function declarations and constants for color filter functions - greyscale, RGB filters, etc.
*/

#ifndef MONO_H
#define MONO_H

#define B 0
#define G 1
#define R 2

#define LUMA_R 0.2126
#define LUMA_G 0.7152
#define LUMA_B 0.0722

void greyscale_filter(Bitmap *bitmap);
void red_filter(Bitmap *bitmap);
void blue_filter(Bitmap *bitmap);
void green_filter(Bitmap *bitmap);

#endif