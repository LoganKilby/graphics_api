#include "platform.h"
#include "app.h"
#include "vector_math.h"
#include "GL/glew.h"

extern "C" __declspec(dllexport) void update_and_render(Application_Memory *memory, Input_State *input) {
    Application_State *app_state = (Application_State *)memory->base_address;
    app_state->scratch_arena.base_address = (u8 *)memory->base_address + sizeof(Application_State);
    app_state->scratch_arena.size = APP_MEMORY_SIZE - sizeof(Application_State);
    app_state->scratch_arena.allocated = 0;
    
    if(!app_state->initialized) {
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
        u8 *vs_source = os_read_entire_file("basic.vs", &app_state->scratch_arena, &vs_source_length);
        OutputDebugStringA((char *)vs_source);
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}