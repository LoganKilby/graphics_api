#include "shared.h"

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
                               "App", // Window text
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
    
#ifdef DEBUG
    assert(CopyFile(APP_DLL_NAME, APP_DLL_NAME_COPY, false));
    AllocConsole();
#endif
    
    HINSTANCE dll_handle;
    Update_And_Render_Ptr update_and_render;
    load_app_dll(&dll_handle, &update_and_render);
    
    // TODO(lmk): Allocate at defined location in memory for debugging
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, APP_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    global_input_state = {};
    
    HDC hdc;
    MSG msg = {};
    while(GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        debug_hot_reload_app_dll(&dll_handle, &update_and_render);
        update_and_render(&app_memory, &global_input_state);
        
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
        } break;
        
        case WM_DESTROY: {
            PostQuitMessage(0);
        }
        
        case WM_CREATE: {
            HDC hdc = GetDC(hwnd);
            win32_set_pixel_format(hdc);
            
            HGLRC hglrc = wglCreateContext(hdc);
            assert(hglrc);
            
            wglMakeCurrent(hdc, hglrc);
            
            GLenum glew_status = glewInit();
            assert(glew_status == GLEW_OK);
        } break;
        
        case WM_MOUSEWHEEL: {
            s32 scroll_delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Scroll;
            event.button = Input_Button::Mouse_MButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position.y = scroll_delta;
            push_input_event(event);
        } break;
        
        case WM_MBUTTONUP: {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Release;
            event.button = Input_Button::Mouse_MButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        case WM_MBUTTONDOWN: {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Press;
            event.button = Input_Button::Mouse_MButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        case WM_RBUTTONDOWN: {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Press;
            event.button = Input_Button::Mouse_RButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        case WM_RBUTTONUP: {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Release;
            event.button = Input_Button::Mouse_RButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        case WM_LBUTTONDOWN:
        {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Press;
            event.button = Input_Button::Mouse_LButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        case WM_LBUTTONUP:
        {
            iv2 mouse_position = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            Input_Event event = {};
            event.device = Input_Device::Mouse;
            event.gesture = Input_Gesture::Release;
            event.button = Input_Button::Mouse_LButton;
            event.sys_modifiers = win32_repack_key_state(wParam);
            event.screen_position = mouse_position;
            push_input_event(event);
        } break;
        
        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    
    return result;
}

internal int win32_set_pixel_format(HDC hdc) {
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
    
#if 0
    DescribePixelFormat(hdc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pixel_struct);
    
    win32_debug_enumerate_pixel_formats(hdc);
#endif
    
    return pixel_format;
}

internal void win32_debug_enumerate_pixel_formats(HDC hdc) {
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

internal __FILETIME win32_get_file_time(char *file_name) {
    HANDLE file_handle = CreateFileA(file_name,
                                     GENERIC_READ,
                                     0,
                                     0,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL,
                                     0);
    
    __FILETIME result = {};
    
    if(file_handle != INVALID_HANDLE_VALUE) {
        BOOL file_time_status = GetFileTime(file_handle, &result.create, &result.access, &result.write);
        assert(file_time_status);
        CloseHandle(file_handle);
    }
    
    return result;
}


internal u32 win32_repack_key_state(WPARAM wParam) {
    u32 result = 0;
    
    u32 win32_key_state = GET_KEYSTATE_WPARAM(wParam);
    
    if(win32_key_state & MK_CONTROL)
        result |= SysMod_Ctrl;
    
    if(win32_key_state & MK_SHIFT)
        result |= SysMod_Shift;
    
    if(GetKeyState(VK_MENU)< 0)
        result |= SysMod_Alt;
    
    return result;
}

internal void win32_hot_reload(HINSTANCE *loaded_dll_handle, Update_And_Render_Ptr *proc_address) {
    __FILETIME copy_dll_file_time = win32_get_file_time(APP_DLL_NAME_COPY);
    __FILETIME base_dll_file_time = win32_get_file_time(APP_DLL_NAME);
    
    if(CompareFileTime(&base_dll_file_time.write, &copy_dll_file_time.write) == 1) {
        FreeLibrary(*loaded_dll_handle);
        BOOL dll_copy_result = CopyFile(APP_DLL_NAME, APP_DLL_NAME_COPY, false);
        assert(dll_copy_result);
        
        *loaded_dll_handle = LoadLibrary(APP_DLL_NAME_COPY);
        *proc_address = (Update_And_Render_Ptr)GetProcAddress(*loaded_dll_handle, "update_and_render");
    }
}

internal bool win32_load_app_dll(char *dll_name, HINSTANCE *dll_handle_out, Update_And_Render_Ptr *proc_address) {
    HINSTANCE dll_handle = LoadLibrary(dll_name);
    assert(dll_handle != INVALID_HANDLE_VALUE); // NOTE(lmk): Could not load dll
    
    if(dll_handle != INVALID_HANDLE_VALUE) {
        *dll_handle_out = dll_handle;
        FARPROC proc = GetProcAddress(dll_handle, "update_and_render");
        assert(proc); // NOTE(lmk): Could not find proc address
        
        *proc_address = (Update_And_Render_Ptr)proc;
        return true;
    }
    
    return false;
}
