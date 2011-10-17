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
#include "bitpack.h"

#define DENOM 255

static void applyCompPrint(int col, int row, A2 array, void* ptr,
                void* cl) {
    (void) row; (void) col; (void) array; (void) cl;
    uint64_t* packedWord = ptr;
    uint64_t word = *packedWord;
    for(int bit = 24; bit >= 0; bit -= 8) {
        int section = Bitpack_getu(word, 8, bit);
        putchar((char)section);
    }
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

    uint64_t* curpix = ptr;
    uint64_t word;

    for(int bit = 24; bit >= 0; bit -= 8) {
        int c = getc(input);
        word = Bitpack_newu(word, 8, bit, c);
    }

    //printf("curpix: %lu\n", word);
    *curpix = word;
}

void decompress40(FILE *input) {
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_mapfun *map = methods->map_default;
    assert(map);

    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width,
        &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    A2 avgDCTPacked = methods->new(width, height, sizeof(uint64_t));
    A2 avgDCTScaled = methods->new(width, height, sizeof(struct AvgDCTScaled));
    A2 avgDCTArray = methods->new(width, height, sizeof(struct AvgDCT));

    width = width*2;
    height = height*2;

    A2 yppArray = methods->new(width, height, sizeof(struct YPP));
    A2 floatArray = methods->new(width, height, sizeof(struct rgbFloat));
    A2 intArray = methods->new(width, height, sizeof(struct Pnm_rgb));

    /* filling array from input */
    methods->map_default(avgDCTPacked, fillToReadArray, input);

    /* creating closures used to fill empty arrays */
    struct Closure cl = { methods, avgDCTPacked, DENOM };
    methods->map_default(avgDCTScaled, applyDecompToAvgDCTScaled, &cl);
    
    cl.array = avgDCTScaled;
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
    methods->free(&avgDCTPacked);
    Pnm_ppmfree(&output);
}
