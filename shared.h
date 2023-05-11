/* date = April 10th 2023 9:35 pm */

#ifndef SHARED_H
#define SHARED_H

#include "D:\Library\glm\glm\glm.hpp"
#include "D:\Library\glm\glm\gtc\constants.hpp"
#include "defines.h"
#include "stdio.h" // for console output

#define APP_MEMORY_SIZE kilobytes(16)

#define SCRATCH_ALLOCATOR scratch_allocate

#if defined(_WIN64)
#define APP_DLL_NAME "app.dll"
#define APP_DLL_NAME_COPY "app.copy.dll"
#include "win32_platform.h"
#include "GL/glew.h"
#include "GL/wglew.h"
#include "opengl_utility/debug.h"
#include "D:/Library/glfw-3.3.8/include/GLFW/glfw3.h"
#define os_read_entire_file(file_name, bytes_read) win32_read_entire_file(file_name, bytes_read)

#elif defined(__APPLE__)
#elif defined(__ANDROID__)
#else
#error No platform defined
#endif

#endif //SHARED_H