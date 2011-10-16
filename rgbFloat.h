#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct rgbFloat {
    float red, green, blue;
}rgbFloat;

extern void compTrimPixmap(Pnm_ppm image);
extern void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                void* ptr, void* cl);
extern void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);
