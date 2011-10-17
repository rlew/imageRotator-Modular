#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct AvgDCTScaled {
    unsigned pb, pr, a;
    int b, c, d;
}AvgDCTScaled;

 /*
 * Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations preformed on the AvgDCTScalled array in the closure.
 */
extern void applyCompToAvgDCTScaled(int col, int row, A2 toBeFilled,
                                    void* ptr, void* cl);

/*
 * Decompression: void* ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the AvgDCTScaled array in the closure.
 */
extern void applyDecompToAvgDCT(int col, int row,
                                A2 toBeFilled, void* ptr, void* cl);
