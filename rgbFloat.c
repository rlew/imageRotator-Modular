#include "rgbFloat.h"
#include "arith40.h"
#include "assert.h"
#include "closure.h"

void compTrimPixmap(Pnm_ppm image) {
    if(image->width % 2 != 0) {
      image->width -= 1;
    }
    if(image->height % 2 != 0) {
      image->height -= 1;
    }
}
/* Compression: void *ptr is a struct of rgbFloat's to be filled with the
 * calculations preformed on the Pnm_rgb array in the closure. */
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

void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
                                                void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct Pnm_rgb* decomped = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array,col, row);
    unsigned int  denominator = mycl->denom;
    //assert(original->red <= 1.0);
    //assert(original->green<= 1.0);
    //assert(original->blue <= 1.0);
    decomped->red = original->red * denominator;
    decomped->green = original->green * denominator;
    decomped->blue = original->blue * denominator;
}


