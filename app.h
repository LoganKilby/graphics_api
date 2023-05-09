/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#ifdef DEBUG
#define SHADER_SOURCE_DIR "../shaders/"
#else
#define SHADER_SOURCE_DIR 
#endif

#define shader_path(name) SHADER_SOURCE_DIR##name

struct Application_State {
    bool initialized;
    
    u32 test_vao;
    u32 test_program;
    
    Memory_Arena scratch_arena;
};

#endif //APP_H
