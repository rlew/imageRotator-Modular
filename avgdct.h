#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct AvgDCT {
    float pb, pr, a, b, c, d;
}AvgDCT;

extern void applyCompToAvgDCT(int col, int row, A2 toBeFilled, 
                              void* ptr, void* cl);
extern void applyDecompToYPP(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);

