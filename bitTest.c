#include "stdlib.h"
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

    //uint64_t width = atoi(argv[1]);
    //uint64_t lsb = atoi(argv[2]);
    //uint64_t value = atoi(argv[3]);
    /*uint64_t unsignedValue;
    printf("%lu %lu %lu\n", width, lsb, value);
    uint64_t i = Bitpack_newu(unsignedValue, width, lsb, value);
    printf("%lu stored in %lu bit: %lu\n", value, lsb,
            Bitpack_getu(i, width, lsb));
*/
    //unsigned u = 20;
    int a = 275;
    uint64_t j = Bitpack_newu(j, 9, 23, a);
    printf("val: %d lsb: %d bit: %ld\n", a, 23,
            Bitpack_getu(j, 9, 23));
    j = Bitpack_news(j, 5, 18, -15);
    //j = Bitpack_newu(j, 5, 13, 31);
    printf("val: %d lsb: %d bit: %ld\n", a, 23,
            Bitpack_getu(j, 9, 23));


    return 0;
}

