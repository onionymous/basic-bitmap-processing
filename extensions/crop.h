/* 
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Header file for crop.c, the crop extension.
*/

#ifndef CROP_H
#define CROP_H

void crop_filter(Bitmap *bitmap, int top, int bottom, int left, int right);

#endif