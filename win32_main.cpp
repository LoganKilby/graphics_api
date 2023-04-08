// Graphics api driver program

#include "win32_platform.h"
#include "defines.h"
#include <d2d1.h>
#include <GL/gl.h>
#include "opengl.h"

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
    
    ShowWindow(hwnd, nCmdShow);
    
    glClearColor(0, 1, 1, 0);
    
    HDC hdc;
    MSG msg = {};
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        hdc = GetDC(hwnd);
        SwapBuffers(hdc);
        ReleaseDC(hwnd, hdc);
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
            
        } break;
        
        case WM_DESTROY: {
            PostQuitMessage(0);
        }
        
        case WM_CREATE: {
            HDC hdc = GetDC(hwnd);
            win32_set_pixel_format(hdc);
            
            HGLRC hglrc = wglCreateContext(hdc);
            win32_assert(hglrc);
            
            wglMakeCurrent(hdc, hglrc);
            
        } break;
        
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    
    return result;
}

static int win32_set_pixel_format(HDC hdc) {
    PIXELFORMATDESCRIPTOR pixel_struct =  {
        sizeof(PIXELFORMATDESCRIPTOR), // nSize
        1, // nVersion
        PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER, // dwFlags
        PFD_TYPE_RGBA, // iPixelType
        32, // cColorBits
        8,  // cRedBits
        0,  // cRedShift
        8,  // cGreenBits
        0,  // cGreenShift
        8,  // cBlueBits
        0,  // cBlueShift
        8,  // cAlphaBits
        0,  // cAlphaShift
        0,  // cAccumBits
        0,  // cAccumRedBits
        0,  // cAccumGreenBits
        0,  // cAccumBlueBits
        0,  // cAccumAlphaBits
        24, // cDepthBits
        8,  // cStencilBits
        0,  // cAuxBuffers
        0,  // iLayerType
        0,  // bReserved
        0,  // dwLayerMask
        0,  // dwVisibleMask
        0,  // dwDamageMask
    };
    
    int pixel_format = ChoosePixelFormat(hdc, &pixel_struct);
    SetPixelFormat(hdc, pixel_format, &pixel_struct);
    
#ifdef DEBUG
    DescribePixelFormat(hdc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pixel_struct);
    
    win32_debug_enumerate_pixel_formats(hdc);
#endif
    
    return pixel_format;
}

static void win32_debug_enumerate_pixel_formats(HDC hdc) {
    // local variables  
    int                      iMax ; 
    PIXELFORMATDESCRIPTOR    pfd; 
    int                      iPixelFormat ; 
    
    // initialize a pixel format index variable  
    iPixelFormat = 1; 
    
    // keep obtaining and examining pixel format data...  
    do { 
        // try to obtain some pixel format data  
        iMax = DescribePixelFormat(hdc, iPixelFormat, sizeof(pfd), &pfd); 
        
        // if there was some problem with that...   
        if (iMax == 0) 
            
            // return indicating failure  
            return; 
    }  
    
    // ...until we've looked at all the device context's pixel formats  
    while (++iPixelFormat <= iMax);
}