#include "stdio.h"
#include "stdlib.h"
#include "bitpack.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t word, unsigned width){
    uint64_t one = 1;
    unsigned rangeMax = one << width;
    if(word >= rangeMax) {
        return false;
    }
    return true;
}

bool Bitpack_fitss(int64_t word, unsigned width) {
    uint64_t one = 1;
    int unsignedRangeMax = one << width;
    int toScaleDown = unsignedRangeMax/2;
    int rangeMax = unsignedRangeMax - toScaleDown;
    int rangeMin = 0 - toScaleDown - 1;

    if(word < rangeMax && word > rangeMin) {
        return true;
    }
    else {
        return false;
    }
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    if(width > 64) RAISE(Bitpack_Overflow);
    if(lsb + width > 64) RAISE(Bitpack_Overflow);

    uint64_t value = word;
    value = value << (64-(width+lsb));
    value = value >> (64-width);

    return value;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    if(width > 64) RAISE(Bitpack_Overflow);
    if(lsb + width > 64) RAISE(Bitpack_Overflow);

    int64_t value = word;
    value = value << (64-(width+lsb));
    value = value >> (64-width);

    return value;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t
value) {
    if(width > 64) RAISE(Bitpack_Overflow);
    if(lsb + width > 64) RAISE(Bitpack_Overflow);
    if(!Bitpack_fitsu(value, width)) RAISE(Bitpack_Overflow);

    uint64_t zeroOpposite = ~0;
    uint64_t leftShift = zeroOpposite << (64-width);
    uint64_t rightShift = leftShift >> (64-lsb-width);
    rightShift = ~rightShift;
    uint64_t spaceCleared = word & rightShift;

    value = value << lsb;

    return spaceCleared | value;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t
value) {
    if(width > 64) RAISE(Bitpack_Overflow);
    if(lsb + width > 64) RAISE(Bitpack_Overflow);
    if(!Bitpack_fitss(value, width)) RAISE(Bitpack_Overflow);

    uint64_t zeroOpposite = ~0;
    uint64_t leftShift = zeroOpposite << (64-width);
    uint64_t rightShift = leftShift >> (64 - lsb - width);
    rightShift = ~rightShift;
    uint64_t spaceCleared = word & rightShift;

    uint64_t valueUnsigned = value << (64 - width);
    valueUnsigned = valueUnsigned >> (64 - lsb - width);

    return spaceCleared | valueUnsigned;
}

