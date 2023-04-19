#include "shared.h"
#include "arena.h"
#include "app.h"
#include "vector_math.h"
#include "GL/glew.h"

extern "C" __declspec(dllexport) void update_and_render(Memory_Arena *platform_memory, Input_State *input) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    assert(sizeof(Application_State) < APP_MEMORY_SIZE); // NOTE(lmk): Increase app memory size
    
    if(!app_state->initialized) {
        scratch_arena = create_arena_local((u8 *)platform_memory->base_address + sizeof(Application_State),
                                           APP_MEMORY_SIZE - sizeof(Application_State));
        glClearColor(0, 1, 0, 0);
        
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        
        
        GLenum glew_status = glewInit();
        assert(glew_status == GLEW_OK);
        
        glGenBuffers(1, &app_state->vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, app_state->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        u32 vs_source_length;
        GLchar* vs_source = (GLchar *)os_read_entire_file("basic.vs", &vs_source_length);
        
        u32 vs_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs_id, 1, &vs_source, 0);
        glCompileShader(vs_id);
        
        s32 vs_compile_status = 0;
        glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_compile_status);
        
        if(vs_compile_status == GL_FALSE) {
            s32 info_log_length = 0;
            glGetShaderiv(vs_compile_status, GL_INFO_LOG_LENGTH, &info_log_length);
            
            if(info_log_length > 0) {
                u8 *info_log = scratch_allocate(info_log_length);
                GLsizei bytes_read;
                glGetShaderInfoLog(vs_id, info_log_length, &bytes_read, vs_source);
            }
        }
    }
    
    scratch_arena.allocated = 0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}