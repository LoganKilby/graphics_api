/* date = March 24th 2023 4:12 pm */

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include "stdio.h" // for console output
#include "defines.h"
#include "Windows.h"
#include "Windowsx.h" // GET_X_PARAM, GET_Y_PARAM
#include "external/glew-2.1.0/GL/glew.h"
#include "external/glew-2.1.0/GL/wglew.h"

#define DEBUG_WORKING_DIR "../../data"
#define os_read_entire_file(file_name, bytes_read) win32_read_entire_file(file_name, bytes_read)
#define APP_DLL_NAME "app.dll"
#define APP_DLL_NAME_COPY "app.copy.dll"
#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

// NOTE(lmk): __cdecl is microsoft-specific

struct __FILETIME {
    FILETIME create;
    FILETIME access;
    FILETIME write;
};

// TODO(lmk): I thought I once saw a way to do this in one line instead of using multiple macros...
#define __merge(a, b) a##b
#define _merge(a, b) __merge(a, b)

#ifdef DEBUG
#define TIMED_BLOCK High_Resolution_Interval _merge(hri, __LINE__)(__FUNCTION__)
#define debug_log(msg) OutputDebugStringA(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME_COPY, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_handle, proc_address) win32_hot_reload(dll_handle, proc_address)
#else
#define TIMED_BLOCK
#define debug_log(msg)
#define load_app_dll(dll_out, proc_address_out) win32_load_app_dll(APP_DLL_NAME, dll_out, proc_address_out)
#define debug_hot_reload_app_dll(dll_out, proc_address_out)
#endif

#define WAS_DOWN(lParam) ((lParam & (1 << 30)) != 0)
#define IS_DOWN(lParam) ((lParam & (1UL << 31)) == 0)


class High_Resolution_Interval
{
	const char* function_name;
    
	LARGE_INTEGER starting_time;
	LARGE_INTEGER frequency;
    
    public: 
	High_Resolution_Interval(const char* caller)
	{
		function_name = caller;
		
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&starting_time);
	}
    
	~High_Resolution_Interval()
	{
		LARGE_INTEGER endingTime;
		QueryPerformanceCounter(&endingTime);
        
		LARGE_INTEGER elapsedMicroseconds;
		elapsedMicroseconds.QuadPart = endingTime.QuadPart - starting_time.QuadPart;
		elapsedMicroseconds.QuadPart *= 1000000;
		elapsedMicroseconds.QuadPart /= frequency.QuadPart;
        
		fprintf(stderr, "%s : %.3Lf ms\n", function_name, elapsedMicroseconds.QuadPart / 1000.0l);
    }
};

u8 *win32_read_entire_file(char *file_name, u32 *bytes_read_out) {
    HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
    u8 *result = 0;
    
    assert(file_handle != INVALID_HANDLE_VALUE); // NOTE(lmk): Failed to open file
    if(file_handle != INVALID_HANDLE_VALUE) {
        
        // TODO(lmk): I'm not sure yet what allocation method to use for reading files.
        // Should the os use the memory allocated to the application instead of the heap?
        u32 file_size = GetFileSize(file_handle, 0);
        result = (u8 *)malloc(file_size);
        memset(result, 0, file_size);
        
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


#if 0 
// NOTE(lmk): win32 platform code. Since this header is shared with the glfw version of the code, I'm leaving this here in case 
// I want to reference it in the future for doing dll loading or switching back to win32.
LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
internal int win32_set_pixel_format(HDC hdc);
internal void win32_debug_enumerate_pixel_formats(HDC hdc);
internal u32 win32_repack_key_state(WPARAM wParam);
typedef void (__cdecl *Update_And_Render_Ptr)(Application_State *);
internal bool win32_load_app_dll(char *dll_name, HINSTANCE *dll_handle_out, Update_And_Render_Ptr *proc_address);
internal void win32_hot_reload(HINSTANCE *loaded_dll_handle, Update_And_Render_Ptr *proc_address);
internal void win32_process_message_queue(Input_Event_List *event_list);

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
#endif


struct OS_Max_Path_String {
    char data[MAX_PATH];
};


bool os_get_open_file_name(char *buffer, size_t buffer_size) {
    char current_dir[MAX_PATH] = {};
    GetCurrentDirectory(sizeof(current_dir), current_dir);
    
    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = "*.scene";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile  = (int)buffer_size;
    ofn.lpstrInitialDir = current_dir;
    ofn.Flags = OFN_FILEMUSTEXIST;
    
    return GetOpenFileName(&ofn);
}


bool os_get_save_file_name(char *buffer, size_t buffer_size) {
    char current_dir[MAX_PATH] = {};
    GetCurrentDirectory(sizeof(current_dir), current_dir);
    
    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = "*.scene";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile  = (int)buffer_size;
    ofn.lpstrInitialDir = current_dir;
    
    return GetSaveFileName(&ofn);
}



#endif //WIN32_PLATFORM_H
