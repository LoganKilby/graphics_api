#include "shared.h"

static bool global_running;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#ifdef DEBUG
    // NOTE(lmk): copying most recent dll build to the duplicate location that gets loaded at runtime
    assert(CopyFile(APP_DLL_NAME, APP_DLL_NAME_COPY, false));
#endif
    
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
                               "win32", // Window text
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
    
    HINSTANCE dll_handle;
    Update_And_Render_Ptr update_and_render;
    load_app_dll(&dll_handle, &update_and_render);
    
    // TODO(lmk): Allocate at defined location in memory for debugging
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, APP_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    Input_State input_state = {};
    
    HDC hdc;
    MSG msg = {};
    
    global_running = true;
    
    while(global_running) {
        debug_hot_reload_app_dll(&dll_handle, &update_and_render);
        win32_process_message_queue(&input_state);
        
        update_and_render(&app_memory, &input_state);
        
        hdc = GetDC(hwnd);
        SwapBuffers(hdc);
        ReleaseDC(hwnd, hdc);
    }
}


internal void win32_process_message_queue(Input_State *input) {
    MSG msg = {};
    
    for(;;) {
        TIMED_BLOCK;
        BOOL got_message = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
        
        if(!got_message) 
            break;
        
        switch(msg.message) {
            case WM_QUIT: {
                global_running = false;
            } break;
            
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP: {
                b32 was_down = WAS_DOWN(msg.lParam);
                b32 is_down = IS_DOWN(msg.lParam);
                b32 alt_key_down = (msg.lParam & (1 << 29));
                b32 shift_key_down = (GetKeyState(VK_SHIFT) & (1 << 15));
                u32 vk_code = (u32)msg.wParam;
                
                if(was_down != is_down) {
                    // TODO(lmk): process keyboard input
                }
                
                if(is_down)
                {
                    if((vk_code == VK_F4) && alt_key_down)
                    {
                        global_running = false;
                    }
                }
                
            } break;
            
            default: {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            } break;
        }
    }
}

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
    switch(uMsg) {
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {
            assert(0);
        } break;
        
        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC device_context = BeginPaint(hwnd, &paint);
            EndPaint(hwnd, &paint);
        } break;
        
        case WM_DESTROY: {
            global_running = false;
        } break;
        
        case WM_CREATE: {
#if DEBUG
            AllocConsole();
            freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
            freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
#endif
            
            HDC hdc = GetDC(hwnd);
            win32_set_pixel_format(hdc);
            
            HGLRC temp_context = wglCreateContext(hdc);
            assert(temp_context);
            
            wglMakeCurrent(hdc, temp_context);
            
            GLenum glew_status = glewInit();
            assert(glew_status == GLEW_OK);
            
            // TODO(lmk): Using the opengl debug output requires Opengl 4.3..
            // Try request a lesser version for running release
            if(wglewIsSupported("WGL_ARB_create_context") == 1) {
                // For the love of god.. I can't figure out how to create a context compatible with fixed pipeline opengl
                int attribs[] = {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
                    0
                };
                
                HGLRC context = wglCreateContextAttribsARB(hdc, 0, attribs);
                
                if(context) {
                    printf("INFO: OpenGL Context Created\n");
                    wglMakeCurrent(0, 0);
                    wglDeleteContext(temp_context);
                    wglMakeCurrent(hdc, context);
                } else {
                    fprintf(stderr, "INFO: Custom OpenGL context failed to create: %d\n", GetLastError());
                }
            } else {
                printf("INFO: WGL_ARB_create_context not supported\n");
            }
            
            int context_flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
            assert(context_flags & GL_CONTEXT_FLAG_DEBUG_BIT);
            if(context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(gl_debug_output, 0);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
            }
            
            int gl_version_major, gl_version_minor;
            glGetIntegerv(GL_MAJOR_VERSION, &gl_version_major);
            glGetIntegerv(GL_MINOR_VERSION, &gl_version_minor);
            printf("%s\n", glGetString(GL_VERSION));
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


internal u32 win32_repack_key_state(WPARAM wParam) {
    u32 result = 0;
    
    u32 win32_key_state = GET_KEYSTATE_WPARAM(wParam);
    
    if(win32_key_state & MK_CONTROL)
        result |= SysMod_Ctrl;
    
    if(win32_key_state & MK_SHIFT)
        result |= SysMod_Shift;
    
    if(GetKeyState(VK_MENU) < 0)
        result |= SysMod_Alt;
    
    return result;
}
