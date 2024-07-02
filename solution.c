// Grant Wilson - 824810214
#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    // Check if File Exists
    if (file == NULL) {
        printf("File does not Exist\n");
        exit(1);
    }

    // Read and validate the PGM header
    char pType[3];
    fscanf(file, "%2s", pType);
    pType[2] = '\0';
    printf("%s \n", pType);

    // Read width and height
    fscanf(file, "%d", width);
    fscanf(file, "%d", height);

    printf("Width: %d  Height: %d \n", *width, *height);


    // Pixel Value Scale? We Don't need anyways
    int notNeeded = 0;
    fscanf(file, "%d", &notNeeded);
    printf("UnNeed: %d \n", notNeeded);

    // Set the Pointer to the beginning of the next line, It is still after the 255
    fgetc(file);

    // Allocate Memory for Height
    PixelGray **image = (PixelGray **)malloc(*height * sizeof(PixelGray *));
    if (image == NULL){
        printf("Error: Unable to allocate memory for rows\n");
        exit(1);
    }

    // Allocate Memory for Width
    for (int i = 0; i < *height; i++) {
        image[i] = (PixelGray *) malloc(*width * sizeof(PixelGray));
        if (image[i] == NULL) {
            printf("Error: Unable to allocate memory for Width in height %d\n", i);
            exit(1);
        }
    }


    // Put Values into Array
        for (int i = 0; i < *height; i++) {
            for (int j = 0; j < *width; j++) {
                unsigned char pixelValue;
                fread(&pixelValue, sizeof(unsigned char), 1, file);
                image[i][j].value = pixelValue;
                printf("%dx%d %d ", i, j, image[i][j].value);
            }
            printf("\n");
        }


        // Close File and Return Array with Values
    fclose(file);
    return image;
}


// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height){
    // Open the file for writing
    FILE* file = fopen(filename, "wb");

    // Check if the file Exists / Opened Correctly
    if (file == NULL) {
        printf("Error: Unable to open the file for writing.\n");
        exit(1);
    }

    // Write the PGM header information
    fprintf(file, "P5\n"); // Always P5 Header
    fprintf(file, "%d %d\n", *width, *height); // From Other Image
    fprintf(file, "255\n"); // We Didn't want this earlier

    // Write the Pixel Values to the File
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            if (fwrite(&matrix[i][j].value, 1, 1, file) != 1) {
                printf("Error: Unable to write pixel data to the file.\n");
                fclose(file);
                exit(1);
            }
        }
    }

    // Close the file
    fclose(file);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
// Create a new 2D matrix for the Threshold Image
    PixelGray** threshMatrix = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    if (threshMatrix == NULL) {
        printf("Error: Unable to allocate memory for thresholded matrix\n");
        exit(1);
    }

    // Allocate Memory for Threshold Matrix
    for (int i = 0; i < *height; i++) {
        threshMatrix[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
        if (threshMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for thresholded matrix\n");
            exit(1);
        }
    }

    // Threshold the Image. Over 80 = 255. Under 80 = 0;
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            if (matrix[i][j].value > 80) {
                threshMatrix[i][j].value = 255; // Set pixel value to 255
            } else {
                threshMatrix[i][j].value = 0;   // Set pixel value to 0
            }
        }
    }

    return threshMatrix;
}


PixelGray** rotate(PixelGray** matrix, int* width, int* height){
    // Create a new 2D matrix for the Rotate Image
    PixelGray** rotateMatrix = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    if (rotateMatrix == NULL) {
        printf("Error: Unable to allocate memory for rotated matrix\n");
        exit(1);
    }

    // Allocate Memory for Rotation Matrix
    for (int i = 0; i < *height; i++) {
        rotateMatrix[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
        if (rotateMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for rotated matrix\n");
            exit(1);
        }
    }

    // Rotation the Image. Swap Rows and Columns
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            rotateMatrix[j][i].value = matrix[i][j].value;
        }
    }

    return rotateMatrix;
}


//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray **image_original = readPGM("lenna.pgm", &width, &height);

    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}
