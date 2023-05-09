#include "shared.h"
#include "app.h"
#include "opengl_utility/core.h"

extern "C" __declspec(dllexport) void update_and_render(Memory_Arena *platform_memory, Input_State *input) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    assert(sizeof(Application_State) < APP_MEMORY_SIZE); // NOTE(lmk): Increase app memory size
    
    if(!app_state->initialized) {
        scratch_arena = create_arena_local((u8 *)platform_memory->base_address + sizeof(Application_State), APP_MEMORY_SIZE - sizeof(Application_State));
        
        gl_utility_init();
        
        app_state->test_vao = gl_create_vao_3f3f();
        
        GL_Utility_Compiled_Shaders sh = {};
        
        u32 src_size;
        char *vert_source = (char *)os_read_entire_file("../shaders/vertex3f3f.vert", &src_size);
        sh.vert = gl_compile_shader(vert_source, src_size, GL_VERTEX_SHADER);
        
        char *frag_source = (char *)os_read_entire_file("../shaders/vertex3f3f.frag", &src_size);
        sh.frag = gl_compile_shader(frag_source, src_size, GL_FRAGMENT_SHADER);
        
        app_state->test_program = gl_link_program(&sh);
        
        glClearColor(0, 1, 0, 0);
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //gl_draw_triangle(v3(0, 0, 0), v3(1, 1, 0), v3(1, 0, 0), v4(1, 0, 0, 1));
    
    
    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    
    glBindVertexArray(app_state->test_vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glUseProgram(app_state->test_program);
    glDrawArrays(GL_TRIANGLES, 0, countof(vertices));
    glBindVertexArray(0);
    
#if 0
    v3 polygon_vertices[] = {
        v3(0.5f, -0.5f, 0.0f),  
        v3(-0.5f, -0.5f, 0.0f), 
        v3(0.0f,  0.5f, 0.0f),
    };
    
    gl_draw_polygon(polygon_vertices, countof(polygon_vertices), v4(1, 0, 0, 1));
#endif
}