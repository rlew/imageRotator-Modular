#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "assert.h"
#include "bitpack.h"

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    if(Bitpack_fitsu(5,3)) printf("5 fits 3 unsigned\n");
    if(!Bitpack_fitsu(8,3)) printf("8 doesn't fit 3 unsigned\n");
    if(Bitpack_fitss(1, 3)) printf("1 fits 3 signed\n");
    if(!Bitpack_fitss(5,3)) printf("5 doesn't fit 3 singed\n");

    if(Bitpack_fitss(-8, 4)) printf("-8 fits 4 signed\n");
    if(!Bitpack_fitss(-100000000000000, 4)) printf("too big doesn't fit\n");
    
    if(!Bitpack_fitsu(5, 0)) printf("unsigned width 0\n");
    if(!Bitpack_fitss(5, 0)) printf("signed width 0\n");
    unsigned long temp = ~7;
    fprintf(stderr, "%lu\n", ~temp); 

    uint64_t tofill;
    uint64_t i = Bitpack_newu(tofill, 3, 0, 1);
    printf("1 stored in 0th bit: %lu\n", Bitpack_getu(i, 3, 0));

    return 0;
}
