<br>
bmpedit
=======

<br>
# Summary

## Description
This program performs simple image processing on bitmap files (threshold, invert colours, greyscale, isolate channel, crop and blur). Its default action is to parse a valid bitmap header and print out the width and height of the bitmap. If filters are specified through command line arguments they will be applied to the image and the resulting bitmap is written to an output file.

## Files and directory structure

The top level directory contains only the main program and the sample bitmap image `cup.bmp`.

#### bmpedit.h
Header file for `bmpedit.c`. Contains function declarations used in the main program, as well as various constants to minimise usage of magic numbers

#### bmpedit.c
The main program that takes the specified command line arguments and calls the appropriate filters based on which options are specified. Command-line argument parsing is done using the `getopt` function from the `<unistd.h>` and `<getopt.h>` libraries.

### Data structures
The `data` folder contains the data structures for the program.

#### bitmap.h
Contains the data structures used to store a bitmap file, function declarations and miscellaneous constant definitions.

`unsigned char` has been typedefed to a `byte`.

`BitmapFileHeader` stores a bitmap file header, i.e. the first 14 bytes of a bitmap file. The first two bytes must be `BM` for it to be a valid bitmap file.

`BitmapInfoHeader` stores a bitmap DIB header (based on the [Windows BITMAPINFOHEADER standard](https://en.wikipedia.org/wiki/BMP_file_format)) and contains information on the pixel data of the bitmap file. 

The header structs require the use of `pragma pack` to stop the OS from automatically padding the struct in memory so that each element begins on a 2 or 4 byte array (as this would cause the structure in memory to not match up with the bitmap header structure and causes issues when writing the header to a file).

`Bitmap` is a struct that stores a bitmap file. It consists of a bitmap file header, a bitmap DIB header, and a pixel array. The pixel array is stored as a one-dimensional array of bytes treated as a two dimensional array, i.e. to get a pixel at a particular row/column: `pixel_array[row*(width*3)+column*3]`

`Pixel` is a struct that represents a single pixel in a bitmap image and contains 3 bytes, i.e. the red, green and blue values.

#### bitmap.c
Contains the implementations of bitmap processing functions.

`new_bitmap()` takes the filename of a bitmap, allocates an appropriate amount of memory, reads the bitmap header and returns a pointer to a `Bitmap` object. By default it will not read the pixel array so all newly-created `Bitmap` objects have a `NULL` pixel array.

`read_pixels()` takes a pointer to an existing `Bitmap` object and a filename. It will obtain the width and height of the image from the `Bitmap` object and read the pixels from the file, row by row, into a malloc'd `byte` array of size `width*height*3` The padding bytes at the end of each row are discarded. The function sets the pixel array of the `Bitmap` object to the newly-read byte array.

`free_bitmap()` frees all the allocated memory for a `Bitmap` object, including the headers, the pixel array and the bitmap itself.

`write_bitmap()` writes an existing `Bitmap` object to a specified output file. The headers are written first, followed by the pixel array, which is written row by row. Padding bytes are added at the end of each row.

`get_padding()` calculates the number of padding bytes for a specified image width.

`get_pixel()` returns the pointer to a `Pixel` object at any specified row and column of a bitmap file. If the row or column is beyond the dimensions of the image, it will wrap around so that it always returns a valid value.

### Extensions
The `extensions` folder contains all the filters, each of which are implemented in an individual file (with a corresponding header file that contains function declarations and some constant definitions) for increased modularity and reusability of code.

+ threshold.h/threshold.c
+ invert.h/invert.c
+ crop.h/crop.c
+ monochrome.h/monochrome.c
+ blur.h/blur.c

For a more detailed explanation of how each filter works and example output images from running each filter, refer to the [Extensions](#filters-extensions) section below.

### Sample images
Sample bitmap images are included in the `bmps` folder as test files for the program.

# How to run
## Compiling
After cloning the git repository, run `make ` in order to compile and build the program. Alternatively, the program can be compiled manually with:
```
gcc -Wall -std=gnu99 -o bmpedit bmpedit.c data/bitmap.c extensions/threshold.c extensions/invert.c extensions/crop.c extensions/monochrome.c extensions/blur.c
```

(`-std=gnu99` is specified because the function `strdup()` from `<string.h>` is required.)

## Running
The program can be run through the command line using: `./bmpedit [OPTIONS] [FILENAME.bmp]`
<br><br>
Basic usage of the program without specifying any options will print out the width and height of the image.
```
$ ./bmpedit cup.bmp
Image width: 640
Image height: 480
```
<br>
Running the program with the `-h` option or with no specified filename will print the following usage message:
```
-------------------------------------------------------------------------------------------------------------------------
DESCRIPTION:
    This program does simple edits of BMP image files. When the program runs it first prints out the width and the height 
    of the input image within the BMP file. Once this is done if there is a filter (or sequence of filters) then they are 
    applied to the image. The resulting image is also stored using BMP format into an output file. Without any filters only 
    the width and height of the image is output.
    
OPTIONS:
    -o FILE                             Sets the output file for modified images (default output file is "out.bmp").
    -t [0.0-1.0]                        Apply a threshold filter to the image with a given threshold value.
    -h                                  Displays this usage message.
    -i                                  Inverts the colours of the image.
    -m [G|r|g|b]                        Applys a monochrome filter to the image. G - greyscale, rgb - isolate colour channel.
    -c [top] [bottom] [left] [right]    Crops an image given the pixels to crop from each side.
    -b [radius]                         Blurs an image given a blur radius.

-------------------------------------------------------------------------------------------------------------------------
```

An output file other than the default `out.bmp` can be specified with the `-o` option, i.e. `-o [OUTFILE.bmp]`

# Filters (Extensions)
## Threshold
The threshold filter takes a floating-point value between 0.0 and 1.0. For every pixel in the bitmap image, if the brightness of the pixel (i.e. the average of the red/green/blue pixel values) is greater than thee threshold value, then the pixel is coloured white. Otherwise, it is coloured black.

Usage:  `./bmpedit -t [number between 0.0 and 1.0] [FILENAME.bmp]`
<br><br>
Example:
```
$ ./bmpedit -t 0.5 -o cupth.bmp cup.bmp
Image width: 640
Image height: 480

Running threshold filter with value: 0.50
```
[<img src="/bmps/cupth.bmp" width="320" />](/bmps/cupth.bmp)

## Invert
The invert filter inverts the colour of every pixel in the bitmap image by performing a bitwise NOT operation.

Usage: `./bmpedit -i [FILENAME.bmp]`
<br><br>
Example:
```
$ ./bmpedit -i -o cupi.bmp cup.bmp
Image width: 640
Image height: 480

Running invert filter
```
[<img src="/bmps/cupi.bmp" width="320" />](/bmps/cupi.bmp)

## Monochrome
The monochrome filter has four different modes:
+ `G`: Greyscale
+ `r`: Isolate red channel
+ `b`: Isolate blue channel
+ `g`: Isolate green channel

The greyscale filter is based off the [ITU-R 601-2 luma transform](https://en.wikipedia.org/wiki/Rec._709) rather than simply taking the average of the RGB pixel values. This corrects for the human eye by taking into account that human vision perceives green more strongly than red and blue and weights each colour component appropriately.

The colour filters set the values of the other two components to zero for every pixel in the bitmap, effectively isolating a single colour channel.

Usage: `./bmpedit -m [MODE] [FILENAME.bmp]`
<br><br>

Examples:

[<img src="/bmps/cupmGrey.bmp" width="200" />](/bmps/cupmGrey.bmp)
&nbsp; &nbsp; &nbsp;
[<img src="/bmps/cupmr.bmp" width="200" />](/bmps/cupmr.bmp)
&nbsp; &nbsp; &nbsp;
[<img src="/bmps/cupmg.bmp" width="200" />](/bmps/cupmg.bmp)
&nbsp; &nbsp; &nbsp;
[<img src="/bmps/cupmb.bmp" width="200" />](/bmps/cupmb.bmp)

## Crop
The crop filter removes a certain number of pixels from each side of the image. It creates a new pixel array after determining the new size of the cropped image, copies the pixel values from the existing pixel array and replaces the bitmap's pixel array. The values for file size, pixel data size, width and height are then updated in the header to ensure that the new bitmap file is valid.

Usage: `./bmpedit -c [TOP] [BOTTOM] [LEFT] [RIGHT] [FILENAME.bmp]`
<br><br>
Example:
```
$ ./bmpedit -c 50 50 50 50 -o cupc.bmp cup.bmp
Image width: 640
Image height: 480

Running crop filter with values:
    Top: 50px
    Bottom: 50px
    Left: 50px
    Right: 50px
```

[<img src="/bmps/cupc.bmp" width="300" />]/bmps/cupc.bmp)

## Blur
The blur function is a simple [box blur](https://en.wikipedia.org/wiki/Box_blur) implementation that calculates the value of each pixel using a convolution kernel that depends on the size of the blur radius specified. The value of the pixel is equal to the average of all the pixels within the blur radius. To improve the efficiency of the algorithm and reduce time complexity by removing repeated calculations, a sliding window approach was implemented. After calculating the sum of the pixels within the sliding window for the first pixel, for all subsequent pixels the window is shifted by one position, so the end pixel is subtracted from the sum and a new pixel is added to the sum. This is implemented for two separate blur functions, a horizontal blur and a vertical blur. When combined, the two functions produce the overall blur effect.

Usage: `./bmpedit -b [RADIUS] [FILENAME.bmp]`
<br><br>
Example:
```
$ ./bmpedit -b 10 -o cupb.bmp cup.bmp
Image width: 640
Image height: 480

Running blur filter with radius: 10
```
[<img src="/bmps/cupb.bmp" width="320" />](/bmps/cupb.bmp)

# Testing
The provided `cup.bmp` file was used extensively during the construction and testing of this program to ensure that it conformed to the assignment specifications. However, as `cup.bmp` had a row width that was a multiple of 4, other bitmaps files with different row widths were created to ensure that the program could adequately handle bitmap row padding. These test files are included in the `bmps` folder.
Black-box testing was employed with several cases to ensure the program behaved as intended under various cases, particularly edge cases (such as the 1x1 bitmap).

## Test cases

+ Testing a case where no filters are applied and the same file is written out <br>
`./bmpedit -o out.bmp cup.bmp` <br>
(Running the `diff` function shows that `out.bmp` and `cup.bmp` are identical, which is the expected outcome.)
+ Testing a 1x1 bitmap
`./bmpedit -o out.bmp bmps/1x1.bmp` <br>
+ Testing other bitmaps of various sizes, particularly those with row sizes that are not a multiple of 4 (to account for padding) <br>
`./bmpedit -o out.bmp -i -t 0.50 bmps/m1.bmp` <br>
`./bmpedit -o out.bmp -c 1 1 1 1 bmps/m2.bmp`
+ Testing combinations of various filters <br>
`./bmpedit -o bmps/mixed.bmp -i -c 50 50 50 50 -b 100 bmps/m2.bmp`
+ Testing the crop function
    + Cropping no pixels off each side <br>
    `./bmpedit -c 0 0 0 0 -o out.bmp cup.bmp`
    + Cropping a multiple of 4 <br>
    `./bmpedit -c 4 4 4 4 -o out.bmp bmps/m1.bmp`
    + Cropping random numbers <br>
    `./bmpedit -c 4 34 1 99 bmps/m1.bmp`
    + Cropping to create a 1x1 bitmap <br>
    `./bmpedit -c 0 479 0 639 cup.bmp`
+ Testing a large blur radius <br>
`./bmpedit -b 480 cup.bmp` <br>
(completely grey image)
    
All the above cases produce valid bitmap files as output and do not result in any errors.

## Using `valgrind` to check for memory leaks

```
$ valgrind --leak-check=full ./bmpedit -c 50 50 50 50 cup.bmp

==17628== Memcheck, a memory error detector
==17628== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==17628== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==17628== Command: ./bmpedit -c 50 50 50 50 cup.bmp
==17628== 
Image width: 640
Image height: 480

Running crop filter with values:
        Top: 50px
        Bottom: 50px
        Left: 50px
        Right: 50px

Input file: cup.bmp
Output file: out.bmp
==17628== 
==17628== HEAP SUMMARY:
==17628==     in use at exit: 0 bytes in 0 blocks
==17628==   total heap usage: 10 allocs, 10 frees, 1,538,950 bytes allocated
==17628== 
==17628== All heap blocks were freed -- no leaks are possible
==17628== 
==17628== For counts of detected and suppressed errors, rerun with: -v
==17628== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```

# Limitations and future improvements

### Doesn't work on big-endian machines
The program is currently intended to work on little-endian machines only. Due to the use of `pragma` and reading directly from a file into a struct, the program does not work on big-endian machines. A more platform-independent solution would be to read the headers into a byte array and extract the values as needed, swapping the bytes if the machine is big-endian.

### Minimal input checking
Currently the program has minimal input checking (for example, checking that the pixels to crop is not more than the width or height of the original image) hence the program can still accept invalid inputs which will result in undefined behaviour. Having stricter input checking (perhaps using `assert` statements) would minimise errors resulting from undefined behaviour.

### Other file types
The program currently only works for bitmap images. Adding support for other popular image formats such as JPEG, PNG or GIF would allow it to become a more general image processing solution.

### Implement Gaussian blur instead of box blur
While box blur is a fast and simple blur implementation, the blur effect produced is not as appealing as the Gaussian blur, which takes a weighted average of the pixels in the convolution kernel (the closer the pixel is to the center, the more highly it is weighted) and produces a more realistic effect.

# References

https://en.wikipedia.org/wiki/BMP_file_format

http://www.cplusplus.com/reference/cstdio/fread

http://www.cplusplus.com/reference/cstring/strerror

http://www.cplusplus.com/references/cerrno
