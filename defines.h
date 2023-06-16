/* date = March 27th 2023 9:18 pm */

#ifndef DEFINES_H
#define DEFINES_H

#define fail(msg) fprintf(stderr, "%s(%d): failed: %s\n", __FUNCTION__, __LINE__, msg); fflush(stderr); assert(0);

#undef assert
#ifdef DEBUG
#define assert(expression) if(!(expression)){ *(int*)0 = 0; }
#else
#define assert(expression) if(!(expression)) { fail("Release Assertion Failed"); }
#endif

#include "stdint.h"

#define internal static
#define global static
#define local static

typedef uint64_t u64;
typedef uint32_t u32;
typedef u32 b32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef float f32;

//#define initialize_internal(this) memset(this, 0, sizeof(*this))
//#define zero_this(this) initialize_internal(this)

// converting to bytes
#define kilobytes(kibs) kibs * 1024
#define megabytes(megs) megs * kilobytes(1024)
#define gigabytes(gigs) gigs * megabytes(1024)
#define countof(array) (sizeof(array) / sizeof(array[0]))

#endif //DEFINES_H
