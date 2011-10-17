#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct AvgDCTScaled {
    unsigned pb, pr, a;
    int b, c, d;
}AvgDCTScaled;

extern void applyCompToAvgDCTScaled(int col, int row, A2 toBeFilled,
                                    void* ptr, void* cl);
extern void applyDecompToAvgDCT(int col, int row,
                                A2 toBeFilled, void* ptr, void* cl);
