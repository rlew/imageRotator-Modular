#include "avgdctscaled.h"
#include "avgdct.h"
#include "assert.h"
#include <math.h>
#include "arith40.h"
#include "closure.h"

/* Compression: converts the arguments of floats and returns a scaled integer in
 * the range of plus or minus 15. */
static int convertToScaledInt(float num) {
  int scaled = nearbyint(50.0*num);
  if (scaled > 15)
    scaled = 15;
  if (scaled < -15)
    scaled = -15;

  return scaled;
}

/* Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations preformed on the AvgDCTScalled array in the closure. */
void applyCompToAvgDCTScaled(int col, int row, A2 toBeFilled, void* ptr,
                              void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct AvgDCTScaled* toBeSet = ptr;
    struct AvgDCT* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->pb = Arith40_index_of_chroma(original->pb);
    toBeSet->pr = Arith40_index_of_chroma(original->pr);
    toBeSet->a = (unsigned)(original->a * 511);
    toBeSet->b = convertToScaledInt(original->b);
    toBeSet->c = convertToScaledInt(original->c);
    toBeSet->d = convertToScaledInt(original->d);
     // assert(toBeSet->b <= 15);
       // assert( toBeSet->b >= -15);
    //assert(toBeSet->c <= 15 || toBeSet->c >= -15);
    //assert(toBeSet->d <= 15);
    //assert(toBeSet->d >= -15);
}

/* Decompression: void* ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the AvgDCTScaled array in the closure */
void applyDecompToAvgDCT(int col, int row, A2 toBeFilled,
    void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct AvgDCT* toBeSet = ptr;
    struct AvgDCTScaled* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->pb = Arith40_chroma_of_index(original->pb);
    toBeSet->pr = Arith40_chroma_of_index(original->pr);
    toBeSet->a = ((float)(original->a))/511.0;
    toBeSet->b = ((float)(original->b))/50.0;
    toBeSet->c = ((float)(original->c))/50.0;
    toBeSet->d = ((float)(original->d))/50.0;
    //if(toBeSet->b > .300000) 
      //printf("ToBeSet b is: %f, row: %d, col %d\n", toBeSet->b, row, col);
    //assert(toBeSet->b <= .300000 );
      //  assert( toBeSet->b >= -.300000);
    //assert(toBeSet->c <= .300000 || toBeSet->c >= -.300000);
    //assert(toBeSet->d >= -.300000 || toBeSet->d <= .300000);
}

