// Graphics api driver program

#include "Windows.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    
    char *window_class_name = "graphics api test";
    WNDCLASSA window_class = {};
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = window_class_name;
    RegisterClass(&window_class);
    
    HWND hwnd = CreateWindowEx(
                               0,                              // Optional window styles.
                               window_class_name,              // Window class
                               "Learn to Program Windows",    // Window text
                               WS_OVERLAPPEDWINDOW,            // Window style
                               
                               // Size and position
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               
                               NULL,       // Parent window    
                               NULL,       // Menu
                               hInstance,  // Instance handle
                               NULL        // Additional application data
                               );
    
    ShowWindow(hwnd, nCmdShow);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
#if 0
    switch(uMsg) {
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
#endif
    
    result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    
    return result;
}