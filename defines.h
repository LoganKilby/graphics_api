/* date = March 27th 2023 9:18 pm */

#ifndef DEFINES_H
#define DEFINES_H

#include "stdint.h"

#ifdef assert
#undef assert
#endif

#ifdef DEBUG
#define assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
#define assert(expression)
#endif

#define internal static
#define global static
#define local static

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

// converting to bytes
#define kilobytes(kibs) kibs * 1024
#define megabytes(megs) megs * kilobytes(1024)
#define gigabytes(gigs) gigs * megabytes(1024)
#define countof(array) (sizeof(array) / sizeof(array[0]))

#endif //DEFINES_H
