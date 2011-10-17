#include "a2methods.h"

#define A2 A2Methods_UArray2

/*
 * Structure that is passed to all apply functions to allow for array
 * manipulation.
 */
typedef struct Closure {
    A2Methods_T methods;
    A2 array;
    unsigned denom;
}Closure;

