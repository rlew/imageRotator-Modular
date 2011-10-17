#include "ypp.h"
#include "rgbFloat.h"
#include "arith40.h"
#include "assert.h"
#include "closure.h"

/*
 * Compression: void *ptr is a struct of YPP to be filled with the calculations
 * performed on the rgbFloat array in the closure.
 */
void applyCompToYPP(int col, int row, A2 toBeFilled, void* ptr,
                                                          void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct YPP* toBeSet = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->y = 0.299 * original->red + 0.587 * original->green
                                                    + 0.114 * original->blue;
    toBeSet->pb = -0.168736 * original->red - 0.331264 * original->green
                                                      + 0.5 * original->blue;
    toBeSet->pr = 0.5 * original->red - 0.418688 * original->green
                                                  - 0.081312 * original->blue;
}


/* 
 * Decompression: void* ptr is a struct of rgbFloat to be filled with the
 * calculations performed on the YPP array in the closrure. 
 */
void applyDecompToRGBFloat(int col, int row, A2 toBeFilled,
                                              void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct rgbFloat *toBeSet = ptr;
    struct YPP* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->red = 1.0 * original->y + 0.0 * original->pb
                                                  + 1.402 * original->pr;
    toBeSet->green = 1.0 * original->y - 0.344136 * original->pb
                                                - 0.714136 * original->pr;
    toBeSet->blue = 1.0 * original->y + 1.772 * original->pb
                                                    + 0.0 * original->pr;
    
    // Checking boundary conditions to prevent negative RGB values
    if(toBeSet->red > 1) toBeSet->red = 1;
    if(toBeSet->red < 0) toBeSet->red = 0;
    if(toBeSet->green > 1) toBeSet->green = 1;
    if(toBeSet->green < 0) toBeSet->green = 0;
    if(toBeSet->blue > 1) toBeSet->blue = 1;
    if(toBeSet->blue < 0) toBeSet->blue = 0;
}


