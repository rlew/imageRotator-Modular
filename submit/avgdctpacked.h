#include "a2methods.h"

#define A2 A2Methods_UArray2

/*
 * Compression: void *ptr is an element of the AvgDCTPacked array that is
 * being filled by the AvgDCTScaled element from the closure.
 */
extern void applyCompToAvgDCTPacked(int col, int row, A2 toBeFilled,
                                    void* ptr, void* cl);
/*
 * Decompression: void *ptr is an element of the AvgDCTScaled array that is
 * being filled by the AvgDCTPacked element from the closure.
 */
extern void applyDecompToAvgDCTScaled(int col, int row,
                                A2 toBeFilled, void* ptr, void* cl);
