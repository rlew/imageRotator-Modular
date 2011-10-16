#include "stdlib.h"
#include "bitpack.h"
#include "assert.h"

bool Bitpack_fitsu(uint64_t word, unsigned width){
    // do 2^width to determine the acceptable range given the num bits
    // verify that word falls within the range
    uint64_t one = 1;
    unsigned rangeMax = one << width;
    // rangeMax undefined when shift overflow occurs
    if(word < rangeMax && word > 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Bitpack_fitss(int64_t n, unsigned width) {
    // do 2^width to determine range and shift down so that it's centered
    // around 0
    // verify that word falls within range
    uint64_t one = 1;
    int unsignedRangeMax = one << width;
    int toScaleDown = unsignedRangeMax/2;
    int rangeMax = unsignedRangeMax - toScaleDown;
    int rangeMin = 0 - toScaleDown - 1;

    if(n < rangeMax && n > rangeMin) {
        return true;
    }
    else {
        return false;
    }
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    assert(width <= 64);
    assert(lsb + width <= 64);
    uint64_t value = word;
    value = value << (64-(width+lsb));
    value = value >> (64-width);
    return value;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t
value) {
    uint64_t zeroOp = ~0;
    uint64_t leftShift = zeroOp << (64-width);
    uint64_t rightShift = leftShift >> (64-width);
    rightShift = ~rightShift;
    uint64_t spaceCleared = word & rightShift;
    value = value << (64-width);
    value = value >> (64-lsb);
    return spaceCleared | value;
}
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t
value);

