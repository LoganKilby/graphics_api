// Graphics api driver program

#include "Windows.h"
#include "win32_platform.h"


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
    
    MSG msg = {};
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
    
    switch(uMsg) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            win32_on_size(&hwnd, (unsigned int)wParam, width, height);
        } break;
        
        case WM_PAINT: {
            win32_on_paint(hwnd);
        } break;
        
        case WM_DESTROY: {
            PostQuitMessage(0);
        }
        
        case WM_CREATE: {
            // Create render target
        } break;
        
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    
    return result;
}