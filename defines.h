/* date = March 27th 2023 9:18 pm */

#ifndef DEFINES_H
#define DEFINES_H

#include "stdint.h"

#ifdef DEBUG
#define assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
#define assert(expression)
#endif

typedef uint64_t u64;

// converting to bytes
#define kilobytes(kibs) kibs * 1024
#define megabytes(megs) megs * kilobytes(1024)
#define gigabytes(gigs) gigs * megabytes(1024)

#endif //DEFINES_H
