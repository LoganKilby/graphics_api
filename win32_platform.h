/* date = March 24th 2023 4:12 pm */

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include "Windows.h"
#include "Windowsx.h" // GET_X_PARAM, GET_Y_PARAM

// NOTE(lmk): __cdecl is microsoft-specific

struct __FILETIME {
    FILETIME create;
    FILETIME access;
    FILETIME write;
};

typedef void (__cdecl *Update_And_Render_Ptr)(Memory_Arena *, Input_State *);

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
internal int win32_set_pixel_format(HDC hdc);
internal void win32_debug_enumerate_pixel_formats(HDC hdc);
internal __FILETIME win32_get_file_time(char *file_name);
internal u32 win32_repack_key_state(WPARAM wParam);
internal bool win32_load_app_dll(char *dll_name, HINSTANCE *dll_handle_out, Update_And_Render_Ptr *proc_address);
internal void win32_hot_reload(HINSTANCE *loaded_dll_handle, Update_And_Render_Ptr *proc_address);

#ifdef DEBUG
#define TIMED_BLOCK High_Resolution_Timer(__FUNCTION__)
#define log(msg) OutputDebugStringA(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME_COPY, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_handle, proc_address) win32_hot_reload(dll_handle, proc_address)
#else
#define TIMED_BLOCK
#define log(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_out, proc_address_out)
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


u8 *win32_read_entire_file(char *file_name, u32 *bytes_read_out) {
    HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
    u8 *result = 0;
    
    assert(file_handle != INVALID_HANDLE_VALUE); // NOTE(lmk): Failed to open file
    if(file_handle != INVALID_HANDLE_VALUE) {
        
        // TODO(lmk): I'm not sure yet what allocation method to use for reading files.
        // Should the os use the memory allocated to the application instead of the heap?
        u32 file_size = GetFileSize(file_handle, 0);
        result = (u8 *)malloc(file_size);
        
        DWORD bytes_read = 0;
        BOOL read_status = ReadFile(file_handle, result, file_size, &bytes_read, 0);
        assert(read_status);
        assert(bytes_read == file_size);
        *bytes_read_out = bytes_read;
    } else {
        *bytes_read_out = 0;
    }
    
    return result;
}


#endif //WIN32_PLATFORM_H
