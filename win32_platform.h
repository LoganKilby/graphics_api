/* date = March 24th 2023 4:12 pm */

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include "Windows.h"

#define WINMSG_MAX_PROCESSING_DURATION 7 // milliseconds.. arbitrarily chosen

class WinMsg_Timer {
    public:
    WinMsg_Timer(char *function_name);
    ~WinMsg_Timer();
    char *caller;
    LARGE_INTEGER start;
    LARGE_INTEGER frequency;
};


#ifdef DEBUG
#define win32_assert(expression) if(!(expression)) {*(int *)0 = 0; }
#define WINMSG_DEBUG_TIMER WinMsg_Timer(__FUNCTION__);
#else
#define win32_assert(expression)
#define WINMSG_DEBUG_TIMER
#endif


WinMsg_Timer::WinMsg_Timer(char *function_name) {
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    caller = function_name;
}


WinMsg_Timer::~WinMsg_Timer() {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    
    LARGE_INTEGER elapsed_microseconds = {};
    elapsed_microseconds.QuadPart = end.QuadPart - start.QuadPart;
    LONGLONG ms = (elapsed_microseconds.QuadPart * 1000000) / frequency.QuadPart;
    
    win32_assert(ms < WINMSG_MAX_PROCESSING_DURATION);
}


static void win32_on_size(HWND *window, unsigned int flag, int width, int height) {
    WINMSG_DEBUG_TIMER;
    return;
}

static void win32_on_paint(HWND hwnd) {
    WINMSG_DEBUG_TIMER;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
    EndPaint(hwnd, &ps);
}

#endif //WIN32_PLATFORM_H
