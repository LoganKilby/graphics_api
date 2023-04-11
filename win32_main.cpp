// Graphics api driver program

#include "defines.h"
#include "platform.h"
#include <d2d1.h>
#include "app.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    char *window_class_name = "graphics api test";
    WNDCLASSA window_class = {};
    window_class.lpfnWndProc = win32_window_proc;
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
    
    HINSTANCE dll_handle = LoadLibrary("app.dll");
    FILETIME current_dll_write = win32_get_file_time("app.dll");
    Update_And_Render_Ptr update_and_render = (Update_And_Render_Ptr)GetProcAddress(dll_handle, "update_and_render");
    
    Application_Memory memory = {};
    memory.size = kilobytes(16);
    memory.base_address = VirtualAlloc(0, memory.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    HDC hdc;
    MSG msg = {};
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        // TODO(lmk): Create a copy of the dll at runtime. Use one for compilation and the other for execution!!
        FILETIME last_dll_write = win32_get_file_time("app.dll");
        if(CompareFileTime(&current_dll_write, &last_dll_write) == 1) {
            FreeLibrary(dll_handle);
            dll_handle = LoadLibrary("app.dll");
            update_and_render = (Update_And_Render_Ptr)GetProcAddress(dll_handle, "update_and_render");
            current_dll_write = last_dll_write;
        }
        
        update_and_render(0);
        
        hdc = GetDC(hwnd);
        SwapBuffers(hdc);
        ReleaseDC(hwnd, hdc);
    }
}


LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
    switch(uMsg) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            //win32_on_size(&hwnd, (unsigned int)wParam, width, height);
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

static FILETIME win32_get_file_time(char *file_name) {
    HANDLE file_handle = CreateFileA(file_name,
                                     GENERIC_READ,
                                     0,
                                     0,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     0);
    
    assert(file_handle != INVALID_HANDLE_VALUE);
    
    FILETIME result = {};
    BOOL file_time_status = GetFileTime(file_handle, 0, 0, &result);
    assert(file_time_status);
    CloseHandle(file_handle);
    
    return result;
}