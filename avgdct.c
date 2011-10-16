#include "avgdct.h"
#include "ypp.h"
#include "arith40.h"
#include "assert.h"
#include "closure.h"

/* Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the YPP array in the closure */
void applyCompToAvgDCT(int col, int row, A2 toBeFilled, void* ptr,
    void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct AvgDCT* toBeSet = ptr;
    struct YPP* original1 = mycl->methods->at(mycl->array, col*2, row*2);
    struct YPP* original2 = mycl->methods->at(mycl->array, col*2 + 1, row*2);
    struct YPP* original3 = mycl->methods->at(mycl->array, col*2, row*2+1);
    struct YPP* original4 = mycl->methods->at(mycl->array, col*2 + 1,
        row*2 + 1);
    toBeSet->pb = (original1->pb + original2->pb + original3->pb +
        original4->pb) / 4.0;
    toBeSet->pr = (original1->pr + original2->pr + original3->pr +
        original4->pr) / 4.0;
    toBeSet->a = (original1->y + original2->y + original3->y +
        original4->y) / 4.0;
    toBeSet->b = (-original1->y - original2->y + original3->y +
        original4->y) / 4.0;
    toBeSet->c = (-original1->y + original2->y - original3->y +
        original4->y) / 4.0;
    toBeSet->d = (original1->y - original2->y - original3->y +
        original4->y) / 4.0;
}


/* Decompression: void *ptr is a struct of YPP to be filled with the
 * calculations performed on the AvgDCT array in the closure. */
void applyDecompToYPP(int col, int row, A2 toBeFilled,
                                                    void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct YPP* toBeSet = ptr;
    struct AvgDCT* original = mycl->methods->at(mycl->array, col/2, row/2);

    if(col%2 == 0 && row%2 == 0)
        toBeSet->y = original->a - original->b - original->c + original->d;
    else if (col%2 == 1 && row%2 == 0)
        toBeSet->y = original->a - original->b + original->c - original->d;

    else if (col%2 == 0 && row%2 == 1)
        toBeSet->y = original->a + original->b - original->c - original->d;
    else
        toBeSet->y = original->a + original->b + original->c + original->d;

    toBeSet->pb = original->pb;
    toBeSet->pr = original->pr;
}


