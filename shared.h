/* date = April 10th 2023 9:35 pm */

#ifndef SHARED_H
#define SHARED_H

#include "defines.h"
#include "arena.h"
#include "input.h"

#define APP_MEMORY_SIZE kilobytes(16)

#define SCRATCH_ALLOCATOR scratch_allocate

#if defined(_WIN64)
#define APP_DLL_NAME "app.dll"
#define APP_DLL_NAME_COPY "app.copy.dll"
#include "win32_platform.h"
#include "GL/glew.h"
#include "GL/wglew.h"
#define os_read_entire_file(file_name, bytes_read) win32_read_entire_file(file_name, bytes_read)

#ifdef DEBUG
#include "stdio.h"
#endif //DEBUG

#elif defined(__APPLE__)
#elif defined(__ANDROID__)
#else
#error No platform defined
#endif

#endif //SHARED_H