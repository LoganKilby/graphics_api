/* date = April 10th 2023 9:35 pm */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "defines.h"

#if defined(_WIN64)
#include "win32_platform.h"
#elif defined(__APPLE__)
#elif defined(__ANDROID__)
#else
#error No platform defined
#endif

struct Application_Memory {
    void *base_address;
    u64 size;
};

#endif //PLATFORM_H