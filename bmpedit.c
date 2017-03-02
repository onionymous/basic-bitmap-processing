/*
COMP2300 Semester 1 2016
Assignment 1 - C Programming: Image Processing
Stephanie Ding (u6076879)

Simple command-line BMP editor.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include "data/bitmap.h"

#include "extensions/threshold.h"
#include "extensions/invert.h"
#include "extensions/crop.h"
#include "extensions/monochrome.h"
#include "extensions/blur.h"

#include "bmpedit.h"

int main(int argc, char* argv[]) {
    // In the case where program is run without any commands
    // Display usage message and exit program
    if (argc < 2) {
        print_usage();
        return EXIT_SUCCESS;
    }

    // Otherwise if command-line arguments have been specified
    // Initialise the flags for all the options
    int flags[NUM_OPTIONS] = { FALSE };
    
    // Set default arguments for the options
    char *file_out = strdup("out.bmp");              // Default output file
    float threshold = 0.5;                           // Default threshold value
    int blur_radius = 1;                             // Default blur radius
    char monochrome_mode = 'G';                      // Default greyscale mode
    int crop_values[CROP_DIMENSIONS] = {0, 0, 0, 0}; // Default crop values

    // Parse the options
    char op;
    while( (op = getopt(argc, argv, "ho:t:im:c:b:")) != -1) {
        switch(op) {
            case 'h':
                flags[OPT_HELP] = TRUE;
                break;
            case 'o':
                flags[OPT_OUT] = TRUE;
                free(file_out);
                file_out = strdup(optarg);
                break;
            case 't':
                flags[OPT_THRESH] = TRUE;
                threshold = atof(optarg);
                break;
            case 'i':
                flags[OPT_INVERT] = TRUE;
                break;
            case 'm':
                if (optarg[0] == 'G' || optarg[0] == 'r' || optarg[0] == 'g' || optarg[0] == 'b') {
                    flags[OPT_MONO] = TRUE;
                    monochrome_mode = optarg[0];
                } else {
                    printf("Invalid argument '%c' for -m. Valid values: G, r, g, b.\n", optarg[0]);
                }
                break;
            case 'c':
                flags[OPT_CROP] = TRUE;

                // Get multiple values for the argument
                optind--;
                int arg_counter = 0;
                while (optind < argc && *argv[optind] != '-' && arg_counter < 4) {
                    crop_values[arg_counter] = atoi(argv[optind]);
                    optind++;
                    arg_counter++;
                }

                // If less than 4 values are specified for crop
                if (arg_counter < 4) {
                    printf("Error: Not enough arguments for option crop.\n");
                    flags[OPT_CROP] = FALSE;
                }

                break;
            case 'b':
                flags[OPT_BLUR] = TRUE;
                blur_radius = atoi(optarg);
                break;
            case ':':
                // Missing option arguments
                fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
                switch (optopt) {
                    case 'o':
                        printf("Setting output file to: out.bmp\n");
                        flags[OPT_OUT] = FALSE;
                        free(file_out);
                        file_out = strdup("out.bmp");
                        break;
                    case 't':
                        printf("Option '-%c' ignored.\n", optopt);
                        flags[OPT_THRESH] = FALSE;
                        break;
                    case 'm':
                        printf("Setting monochrome mode to: G (greyscale)\n");
                        monochrome_mode = 'G';
                        break;
                    case 'c':
                        printf("Option '-%c' ignored.\n", optopt);
                        flags[OPT_CROP] = FALSE;
                        break;
                    case 'b':
                        printf("Option '-%c' ignored.\n", optopt);
                        flags[OPT_BLUR] = FALSE;
                        break;
                }
                break;
            case '?':
                // Invalid option
                fprintf(stderr, "Option '-%c' is an invalid option.\n", optopt);
                break;
        }
    }

    // Get the filename
    char *file_in = NULL;
    for (int i = optind; i < argc; i++) {
        if (!file_in) {
            file_in = strdup(argv[i]);
        } else {
            printf("Unrecognised non-option argument: %s\n", argv[i]);
        }
    }

    // If no file was specified, display usage message and exit the program
    if (!file_in) {
        printf("No input file specified.\n");
        print_usage();
        return EXIT_SUCCESS;
    }

    // Otherwise, read the bitmap image
    Bitmap *bitmap = new_bitmap(file_in);
    read_pixels(bitmap, file_in);

    // Default behaviour is to print out the width and height
    print_dimensions(bitmap);
    
    // Run image processing functions depending on specified options

    if(flags[OPT_HELP] == TRUE) {
        // -h option specified, print usage
        print_usage();
    }

    if(flags[OPT_CROP] == TRUE) {
        // -c [top] [bottom] [left] [right] specified, run crop filter
        printf("\nRunning crop filter with values:\n");
        printf("\tTop: %dpx\n", crop_values[0]);
        printf("\tBottom: %dpx\n", crop_values[1]);
        printf("\tLeft: %dpx\n", crop_values[2]);
        printf("\tRight: %dpx\n", crop_values[3]);
        crop_filter(bitmap, crop_values[0], crop_values[1], crop_values[2], crop_values[3]);
    }

    if(flags[OPT_THRESH] == TRUE) {
        // -t [threshold] specified, run threshold filter
        printf("\nRunning threshold filter with value %.2f\n", threshold);
        threshold_filter(bitmap, threshold);

    }

    if(flags[OPT_INVERT] == TRUE) {
        // -i specified, run invert filter
        printf("\nRunning invert filter\n");
        invert_filter(bitmap);
    }

    if(flags[OPT_MONO] == TRUE) {
        // -m [G|r|g|b] specified, run monochrome filter
        printf("\nRunning monochrome filter in mode: %c\n", monochrome_mode);
        switch (monochrome_mode) {
            case 'G':
                greyscale_filter(bitmap);
                break;
            case 'r':
                red_filter(bitmap);
                break;
            case 'g':
                green_filter(bitmap);
                break;
            case 'b':
                blue_filter(bitmap);
                break;
        }
    }

    if(flags[OPT_BLUR] == TRUE) {
        // -b [radius] specified, run blur filter
        printf("\nRunning blur filter with radius: %d\n", blur_radius);
        horizontal_blur(bitmap, blur_radius);
        vertical_blur(bitmap, blur_radius);
    }

    // Print information about input and output file:
    printf("\nInput file: %s\n", file_in);
    printf("Output file: %s\n", file_out);

    // Write the processed bitmap
    write_bitmap(bitmap, file_out);

    // Free the bitmap
    free_bitmap(bitmap);

    // Free the hidden-malloc'd strings
    free(file_in);
    free(file_out);

    return EXIT_SUCCESS;
}

void print_dimensions(Bitmap *bitmap) {
    printf("Image width: %d\n", bitmap->info_header->width);
    printf("Image height: %d\n", bitmap->info_header->height);
}

void print_usage(void) {
    char *linebreak = "-------------------------------------------------------------------------------------------------------------------------";
    char *description = 
    "\tThis program does simple edits of BMP image files. When the program runs it first prints\n\tout the width and the height of the input image within the BMP file. Once this is done if\n\tthere is a filter (or sequence of filters) then they are applied to the image. The resulting\n\timage is also stored using BMP format into an output file. Without any filters only the\n\twidth and height of the image is output.";
    char *option_names[NUM_OPTIONS] = {
        "-o FILE",
        "-t [0.0-1.0]",
        "-h",
        "-i",
        "-m [G|r|g|b]",
        "-c [top] [bottom] [left] [right]",
        "-b [radius]"
    };
    char *options_text[NUM_OPTIONS] = {
        "Sets the output file for modified images (default output file is \"out.bmp\").",
        "Apply a threshold filter to the image with a given threshold value.",
        "Displays this usage message",
        "Inverts the colours of the image.",
        "Applys a monochrome filter to the image. G - greyscale, rgb - isolate colour channel.",
        "Crops an image given the pixels to crop from each side.",
        "Blurs an image given a blur radius."
    };
    printf("\n%s\n", linebreak);
    printf("Usage: bmpedit [OPTIONS...] [input.bmp]\n\n");
    printf("DESCRIPTION:\n");
    printf("%s\n", description);
    printf("\n");
    printf("OPTIONS:\n");
    for (int i = 0; i < NUM_OPTIONS; i++) {
        printf("\t%-40s %s\n", option_names[i], options_text[i]);
    }
    printf("%s\n", linebreak);
}