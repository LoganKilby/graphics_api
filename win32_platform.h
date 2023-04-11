/* date = March 24th 2023 4:12 pm */

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include "Windows.h"
#include "stdint.h"

// NOTE(lmk): __cdecl is microsoft-specific

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static int win32_set_pixel_format(HDC hdc);
static void win32_debug_enumerate_pixel_formats(HDC hdc);
static FILETIME win32_get_file_time(char *file_name);

#define WINMSG_MAX_PROCESSING_DURATION 7 // milliseconds.. arbitrarily chosen

#ifdef DEBUG
#define win32_assert(expression) if(!(expression)) {*(int *)0 = 0; }
#define TIMED_BLOCK High_Resolution_Timer(__FUNCTION__)
#else
#define win32_assert(expression)
#define TIMED_BLOCK
#endif

class High_Resolution_Timer {
    public:
    High_Resolution_Timer(char *function_name);
    ~High_Resolution_Timer();
    char *caller;
    LARGE_INTEGER start;
    LARGE_INTEGER frequency;
};


High_Resolution_Timer::High_Resolution_Timer(char *function_name) {
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    caller = function_name;
}


High_Resolution_Timer::~High_Resolution_Timer() {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    
    LARGE_INTEGER elapsed_microseconds = {};
    elapsed_microseconds.QuadPart = end.QuadPart - start.QuadPart;
    LONGLONG ms = (elapsed_microseconds.QuadPart * 1000000) / frequency.QuadPart;
}

#endif //WIN32_PLATFORM_H
