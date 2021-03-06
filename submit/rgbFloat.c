#include "rgbFloat.h"
#include "arith40.h"
#include "assert.h"
#include "closure.h"

/*
 * Compression: trimming the outer pixel rows and columns to make dimensions
 * even
 */
void compTrimPixmap(Pnm_ppm image) {
    assert(image);
    if(image->width % 2 != 0) {
      image->width -= 1;
    }
    if(image->height % 2 != 0) {
      image->height -= 1;
    }
}
/*
 * Compression: void *ptr is a struct of rgbFloat's to be filled with the
 * calculations performed on the Pnm_rgb array in the closure.
 */
void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct rgbFloat* toBeSet = ptr;
    struct Pnm_rgb* original = mycl->methods->at(mycl->array, col, row);
    float denom = (float)mycl->denom;
    toBeSet->red = ((float)original->red) / denom;
    toBeSet->green = ((float)original->green) / denom;
    toBeSet->blue = ((float)original->blue) / denom;
}

/*
 * Decompression: void *ptr is a struct of RGB integers to be filled with the
 * calculations performed on the rgbFloat array in the closure
 */
void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
                                                void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct Pnm_rgb* decomped = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array,col, row);
    unsigned int  denominator = mycl->denom;

    // Checking boundary conditions to prevent negative RGB values
    if(original->red < 0) original->red = 0;
    if(original->green < 0) original->green = 0;
    if(original->blue < 0) original->blue = 0;

    decomped->red = original->red * denominator;
    decomped->green = original->green * denominator;
    decomped->blue = original->blue * denominator;

    if(decomped->red > 255) decomped->red = 255;
    if(decomped->green > 255) decomped->green = 255;
    if(decomped->blue > 255) decomped->blue = 255;
}


