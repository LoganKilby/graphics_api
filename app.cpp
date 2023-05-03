#include "shared.h"
#include "arena.h"
#include "app.h"
#include "vector_math.h"
#include "opengl_utility/shader.h"

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
        
        glGenBuffers(1, &app_state->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, app_state->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        u32 source_length;
        GLchar* vs_source = (GLchar *)os_read_entire_file(shader_path("basic.vs"), &source_length);
        GLint vs_id = compile_shader(vs_source, source_length, GL_VERTEX_SHADER);
        
        app_state->initialized = true;
    }
    
    scratch_arena.allocated = 0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glBegin(GL_POLYGON);
    glVertex3f(200.0, 20.0, 0.0);
    glVertex3f(800.0, 20.0, 0.0);
    glVertex3f(303.0, 220.0, 0.1);
    glEnd();
}