#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "stdlib.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "rgbFloat.h"
#include "ypp.h"
#include "avgdct.h"
#include "avgdctscaled.h"
#include "avgdctpacked.h"
#include "closure.h"

#define DENOM 255

static void applyCompPrint(int col, int row, A2 array, A2Methods_Object* ptr,
                void* cl) {
    (void) col; (void) row; (void) array; (void) cl;
    
    
    //struct AvgDCTScaled* elem = ptr;

    //fprintf(stdout, "%u %u %u %d %d %d ", elem->pb, elem->pr, elem->a,
    //                                   elem->b, elem->c, elem->d);
}

static void compWrite(A2 array, A2Methods_T methods) {
    unsigned height = (unsigned)methods->height(array);
    unsigned width = (unsigned)methods->width(array);
    fprintf(stdout, "COMP40 Compressed image format 2\n%u %u\n", width,
        height);
    methods->map_default(array, applyCompPrint, NULL);
}

void compress40(FILE *input) {
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_mapfun *map = methods->map_default;
    assert(map);
    #define SET_METHODS(METHODS,MAP,WHAT) do {\
        methods = (METHODS); \
        assert(methods); \
        map = methods->MAP; \
        if(!map) { \
            frprintf(stderr, "%s does not support " WHAT " mapping\n", \
            argv[0]); \
            exit(1); \
        } \
    } while(0)
    Pnm_ppm image;
    TRY
        image = Pnm_ppmread(input, methods);
    EXCEPT(Pnm_Badformat)
        fprintf(stderr, "Badly formatted file.\n");
        exit(1);
    END_TRY;

    /* Trim the pixmap if it's width or height is odd */
    compTrimPixmap(image);

    /* Convert RGB Ints to RGB Floats */
    A2 floatArray = methods->new(image->width, image->height,
                                sizeof(struct rgbFloat));
    struct Closure cl = { methods, image->pixels, image->denominator };
    methods->map_default(floatArray, applyCompToRGBFloat, &cl);

    // Convert from rgbFloat to YPP
    A2 yppArray = methods->new(methods->width(floatArray),
                               methods->height(floatArray),
                               sizeof(struct YPP));
    cl.array = floatArray;
    methods->map_default(yppArray, applyCompToYPP, &cl);

    // Convert from YPP to AvgDCT
    A2 avgDCTArray = methods->new((methods->width(yppArray))/2,
                                  (methods->height(yppArray))/2,
                                  sizeof(struct AvgDCT));
    cl.array = yppArray;
    methods->map_default(avgDCTArray, applyCompToAvgDCT, &cl);

    // Convert from AvgDCT to AvgDCTScaled
    A2 avgDCTScaledArray = methods->new(methods->width(avgDCTArray),
                                        methods->height(avgDCTArray),
                                        sizeof(struct AvgDCTScaled));
    cl.array = avgDCTArray;
    methods->map_default(avgDCTScaledArray, applyCompToAvgDCTScaled, &cl);


    // Convert from AvgDCTScaled to AvgDCTPacked Array
    A2 avgDCTPackedArray = methods->new(methods->width(avgDCTScaledArray), 
                                   methods->height(avgDCTScaledArray),
                                   sizeof(uint64_t));
    cl.array = avgDCTScaledArray;
    methods->map_default(avgDCTPackedArray, applyCompToAvgDCTPacked, &cl);

    /* Writing functions to stdout */
    compWrite(avgDCTPackedArray, methods);

    /* Freeing functions */
    methods->free(&avgDCTPackedArray);
    methods->free(&avgDCTScaledArray);
    methods->free(&avgDCTArray);
    methods->free(&yppArray);
    methods->free(&floatArray);
    Pnm_ppmfree(&image);
}



/*---------------------------------------------------------------------------*/

/* this function only works with Avg and DCT struct */
static void fillToReadArray(int col, int row, A2 array, A2Methods_Object* ptr,
    void* cl) {
    (void) col; (void) row; (void) array;
    FILE* input = cl;

    struct AvgDCTScaled* curpix = ptr;
    struct AvgDCTScaled elem;

    fscanf(input, "%u", &elem.pb);
    fscanf(input, "%u", &elem.pr);
    fscanf(input, "%u", &elem.a);
    fscanf(input, "%d", &elem.b);
    fscanf(input, "%d", &elem.c);
    fscanf(input, "%d", &elem.d);

    *curpix = elem;
}

void decompress40(FILE *input) {
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_mapfun *map = methods->map_default;
    assert(map);
    #define SET_METHODS(METHODS,MAP,WHAT) do {\
        methods = (METHODS); \
        assert(methods); \
        map = methods->MAP; \
        if(!map) { \
            frprintf(stderr, "%s does not support " WHAT " mapping\n", \
            argv[0]); \
            exit(1); \
        } \
    } while(0)
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width,
        &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    A2 avgDCTScaled = methods->new(width, height, sizeof(struct AvgDCTScaled));
    A2 avgDCTArray = methods->new(width, height, sizeof(struct AvgDCT));

    width = width*2;
    height = height*2;

    A2 yppArray = methods->new(width, height, sizeof(struct YPP));
    A2 floatArray = methods->new(width, height, sizeof(struct rgbFloat));
    A2 intArray = methods->new(width, height, sizeof(struct Pnm_rgb));

    /* filling arrays from input */
    methods->map_default(avgDCTScaled, fillToReadArray, input);

    /* creating closures used to fill empty arrays */
    struct Closure cl = { methods, avgDCTScaled, DENOM };

    //cl.array = avgDCTScaled;
    methods->map_default(avgDCTArray, applyDecompToAvgDCT, &cl);

    cl.array = avgDCTArray;
    methods->map_default(yppArray, applyDecompToYPP, &cl);

    cl.array = yppArray;
    methods->map_default(floatArray, applyDecompToRGBFloat, &cl);

    cl.array = floatArray;
    methods->map_default(intArray, applyDecompToRGBInt, &cl);

    /* Outputing to Ppm */
    Pnm_ppm output;
    NEW(output);
    output->denominator = DENOM;
    output->width = width;
    output->height = height;
    output->pixels = intArray;
    output->methods = methods;

    Pnm_ppmwrite(stdout, output);

    /* Freeing functions */
    methods->free(&floatArray);
    methods->free(&yppArray);
    methods->free(&avgDCTArray);
    methods->free(&avgDCTScaled);
    Pnm_ppmfree(&output);
}
