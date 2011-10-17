#include "a2methods.h"
#define A2 A2Methods_UArray2

typedef struct YPP {
    float y, pb, pr;
}YPP;

extern void applyCompToYPP(int col, int row, A2 toBeFilled, 
                           void* ptr, void* cl);
extern void applyDecompToRGBFloat(int col, int row, A2 toBeFilled,
                                  void* ptr, void* cl);

