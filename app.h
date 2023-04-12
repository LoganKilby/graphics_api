/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

typedef void (__cdecl *Update_And_Render_Ptr)(Application_Memory *, Input_State *);

#define APP_MEMORY_SIZE kilobytes(16)

struct Application_State {
    bool initialized;
    u32 vbo;
    
    Arena scratch_arena;
};

OS_Read_Entire_File_Ptr os_read_entire_file;

#endif //APP_H
