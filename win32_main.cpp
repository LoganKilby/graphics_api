// Graphics api driver program

#include "win32_platform.h"
#include "defines.h"
#include <d2d1.h>
#include <GL/gl.h>
#include "opengl.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static int win32_set_pixel_format(HDC);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    char *window_class_name = "graphics api test";
    WNDCLASSA window_class = {};
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = window_class_name;
    RegisterClass(&window_class);
    
    HWND hwnd = CreateWindowEx(
                               // Optional window styles
                               CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
                               window_class_name,          // Window class
                               "Learn to Program Windows", // Window text
                               WS_OVERLAPPEDWINDOW,        // Window style
                               
                               // Size and position
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               
                               NULL,      // Parent window    
                               NULL,      // Menu
                               hInstance, // Instance handle
                               NULL       // Additional application data
                               );
    
    assert(hwnd);
    
    HDC hdc = GetDC(hwnd);
    win32_set_pixel_format(hdc);
    HGLRC rendering_context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, rendering_context);
    ReleaseDC(hwnd, hdc);
    
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
            
        } break;
        
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    
    return result;
}

static int win32_set_pixel_format(HDC hdc) {
    WINMSG_DEBUG_TIMER;
    
    PIXELFORMATDESCRIPTOR pixel_struct =  {
        sizeof(PIXELFORMATDESCRIPTOR), // nSize
        1, // nVersion
        PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER, // dwFlags
        PFD_TYPE_RGBA, // iPixelType
        24, // cColorBits
        0,  // cRedShift
        0,  // cGreenBits
        0,  // cGreenShift
        0,  // cBlueBits
        0,  // cBlueShift
        0,  // cAlphaBits
        0,  // cAlphaShift
        0,  // cAccumBits
        0,  // cAccumRedBits
        0,  // cAccumGreenBits
        0,  // cAccumBlueBits
        0,  // cAccumAlphaBits
        32, // cDepthBits
        32, // cStencilBits
        0,  // cAuxBuffers
        0,  // iLayerType
        0,  // bReserved
        0,  // dwLayerMask
        0,  // dwVisibleMask
        0,  // dwDamageMask
    };
    
    int pixel_format = ChoosePixelFormat(hdc, &pixel_struct);
    SetPixelFormat(hdc, pixel_format, &pixel_struct);
    
    return pixel_format;
}

static int win32_debug_enumerate_pixel_formats(HDC hdc) {
    int pixel_format_count;
    int pixel_format_index = 0;
    PIXELFORMATDESCRIPTOR ppfd;
    
    do {
        pixel_format_count = DescribePixelFormat(hdc, pixel_format_index++, sizeof(PIXELFORMATDESCRIPTOR), &ppfd);
        
        if(pixel_format_count == 0)
            break;
        
    } while(pixel_format_index < pixel_format_count);
    
    return pixel_format_count;
}