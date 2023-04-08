/* date = March 24th 2023 4:12 pm */

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include "Windows.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static int win32_set_pixel_format(HDC hdc);
static void win32_debug_enumerate_pixel_formats(HDC hdc);

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


static void win32_on_size(HWND *window, unsigned int flag, int width, int height) {
    return;
}

static void win32_on_paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
    EndPaint(hwnd, &ps);
}

#endif //WIN32_PLATFORM_H
