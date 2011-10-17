#include "a2methods.h"

#define A2 A2Methods_UArray2

extern void applyCompToAvgDCTPacked(int col, int row, A2 toBeFilled,
                                    void* ptr, void* cl);
extern void applyDecompToAvgDCTScaled(int col, int row,
                                A2 toBeFilled, void* ptr, void* cl);
