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

#ifdef DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#define TIMED_BLOCK High_Resolution_Timer(__FUNCTION__)
#define log(msg) OutputDebugStringA(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME_COPY, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_handle, proc_address) win32_hot_reload(dll_handle, proc_address)
#else
#define TIMED_BLOCK
#define log(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_out, proc_address_out)
#define assert(expression)
#endif

#include "arena.h"
#include "input.h"
typedef void (__cdecl *Update_And_Render_Ptr)(Memory_Arena *, Input_State *);

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
internal int win32_set_pixel_format(HDC hdc);
internal void win32_debug_enumerate_pixel_formats(HDC hdc);
internal u32 win32_repack_key_state(WPARAM wParam);
internal bool win32_load_app_dll(char *dll_name, HINSTANCE *dll_handle_out, Update_And_Render_Ptr *proc_address);
internal void win32_hot_reload(HINSTANCE *loaded_dll_handle, Update_And_Render_Ptr *proc_address);
internal void win32_process_message_queue(Input_State *state);

#define WAS_DOWN(lParam) ((lParam & (1 << 30)) != 0)
#define IS_DOWN(lParam) ((lParam & (1UL << 31)) == 0)

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
        result = (u8 *)SCRATCH_ALLOCATOR(file_size);
        
        DWORD bytes_read = 0;
        BOOL read_status = ReadFile(file_handle, result, file_size, &bytes_read, 0);
        assert(read_status);
        assert(bytes_read == file_size);
        *bytes_read_out = bytes_read;
        
        CloseHandle(file_handle);
    } else {
        *bytes_read_out = 0;
    }
    
    return result;
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


internal void win32_hot_reload(HINSTANCE *loaded_dll_handle, Update_And_Render_Ptr *proc_address) {
    __FILETIME copy_dll_file_time = win32_get_file_time(APP_DLL_NAME_COPY);
    __FILETIME base_dll_file_time = win32_get_file_time(APP_DLL_NAME);
    
    if(CompareFileTime(&base_dll_file_time.write, &copy_dll_file_time.write) == 1) {
        FreeLibrary(*loaded_dll_handle);
        BOOL dll_copy_result = CopyFile(APP_DLL_NAME, APP_DLL_NAME_COPY, false);
        assert(dll_copy_result);
        
        *loaded_dll_handle = LoadLibrary(APP_DLL_NAME_COPY);
        *proc_address = (Update_And_Render_Ptr)GetProcAddress(*loaded_dll_handle, "update_and_render");
        fprintf(stderr, "INFO: Reloaded %s\n", APP_DLL_NAME_COPY);
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


#endif //WIN32_PLATFORM_H
