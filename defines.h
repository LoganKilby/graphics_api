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

// converting to bytes
#define kilobytes(kibs) kibs * 1024
#define megabytes(megs) megs * kilobytes(1024)
#define gigabytes(gigs) gigs * megabytes(1024)
#define countof(array) (sizeof(array) / sizeof(array[0]))
#define EPSILON 0.001f
#define FULL_CIRCLE 2.0f * PI
#define QUARTER_CIRCLE (PI / 2.0f)
#define MAX_POLAR_ROTATION (QUARTER_CIRCLE - EPSILON)
#define UP v3(0, 1, 0)

#endif //DEFINES_H
