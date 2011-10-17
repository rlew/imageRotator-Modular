#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct AvgDCT {
    float pb, pr, a, b, c, d;
}AvgDCT;
/*
 * Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the YPP array in the closure
 */
extern void applyCompToAvgDCT(int col, int row, A2 toBeFilled,
                              void* ptr, void* cl);
/*
 * Decompression: void *ptr is a struct of YPP to be filled with the
 * calculations performed on the AvgDCT array in the closure.
 */
extern void applyDecompToYPP(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);

