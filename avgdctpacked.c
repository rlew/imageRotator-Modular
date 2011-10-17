#include "avgdctpacked.h"
#include "stdint.h"
#include "closure.h"
#include "bitpack.h"
#include "avgdctscaled.h"

/*
 * Compression: void *ptr is an element of the AvgDCTPacked array that is
 * being filled by the AvgDCTScaled element from the closure.
 */
void applyCompToAvgDCTPacked(int col, int row, A2 toBeFilled,
                                    void* ptr, void* cl){
    (void) toBeFilled;
    uint64_t* toBeSet = ptr;
    struct Closure* mycl = cl;
    struct AvgDCTScaled* original = mycl->methods->at(mycl->array, col, row);
    uint64_t word;
    
    word = Bitpack_newu(word, 9, 23, original->a);
    word = Bitpack_news(word, 5, 18, original->b);
    word = Bitpack_news(word, 5, 13, original->c);
    word = Bitpack_news(word, 5, 8, original->d);
    word = Bitpack_newu(word, 4, 4, original->pb);
    word = Bitpack_newu(word, 4, 0, original->pr);
    
    toBeSet = &word; 
}

/*
 * Decompression: void *ptr is an element of the AvgDCTScaled array that is
 * being filled by the AvgDCTPacked element from the closure.
 */
void applyDecompToAvgDCTScaled(int col, int row,
                                A2 toBeFilled, void* ptr, void* cl){
    (void) toBeFilled;
    struct AvgDCTScaled* toBeSet = ptr;
    struct Closure* mycl = cl;
    uint64_t* word = mycl->methods->at(mycl->array, col, row);
    uint64_t original = *word;

    toBeSet->a = Bitpack_getu(original, 9, 23);
    toBeSet->b = Bitpack_gets(original, 5, 18);
    toBeSet->c = Bitpack_gets(original, 5, 13);
    toBeSet->d = Bitpack_gets(original, 5, 8);
    toBeSet->pb = Bitpack_getu(original, 4, 4);
    toBeSet->pr = Bitpack_getu(original, 4, 0);
}
