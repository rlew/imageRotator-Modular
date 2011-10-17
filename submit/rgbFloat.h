#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct rgbFloat {
    float red, green, blue;
}rgbFloat;

/* Checked runtime error to pass a null image. Makes the dimensions of the image
 * even.*/
extern void compTrimPixmap(Pnm_ppm image);
/*
 * Compression: void *ptr is a struct of rgbFloat's to be filled with the
 * calculations performed on the Pnm_rgb array in the closure.
 */

extern void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                void* ptr, void* cl);/*
 * Decompression: void *ptr is a struct of RGB integers to be filled with the
 * calculations performed on the rgbFloat array in the closure
 */
extern void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);
