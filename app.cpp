#include "shared.h"
#include "app.h"

extern "C" __declspec(dllexport) void update_and_render(Memory_Arena *platform_memory, Input_State *input) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    assert(sizeof(Application_State) < APP_MEMORY_SIZE); // NOTE(lmk): Increase app memory size
    
    gl_utility_context_ptr = &app_state->gl_utility_context;
    
    if(!app_state->initialized) {
        scratch_arena = create_arena_local((u8 *)platform_memory->base_address + sizeof(Application_State), APP_MEMORY_SIZE - sizeof(Application_State));
        
        gl_utility_init(&app_state->gl_utility_context);
        
        gl_vertex_buffer_3f3f(&app_state->test_vao, &app_state->test_vbo);
        
        GL_Utility_Compiled_Shaders sh = {};
        
        u32 src_size;
        char *vert_source = (char *)os_read_entire_file("../shaders/vertex3f3f.vert", &src_size);
        sh.vert = gl_compile_shader(vert_source, src_size, GL_VERTEX_SHADER);
        char *frag_source = (char *)os_read_entire_file("../shaders/vertex3f3f.frag", &src_size);
        sh.frag = gl_compile_shader(frag_source, src_size, GL_FRAGMENT_SHADER);
        app_state->test_program = gl_link_program(&sh);
        
        vert_source = (char *)os_read_entire_file("../shaders/v3f_uv2f.vert", &src_size);
        sh.vert = gl_compile_shader(vert_source, src_size, GL_VERTEX_SHADER);
        frag_source = (char *)os_read_entire_file("../shaders/texture_mix_v3f_uv2f.frag", &src_size);
        sh.frag = gl_compile_shader(frag_source, src_size, GL_FRAGMENT_SHADER);
        app_state->texture_mix_program = gl_link_program(&sh);
        
        glClearColor(0.2, 0.2, 0.2, 0);
        
        gl_vertex_buffer_3f2f(&app_state->v3f2f);
        
        app_state->alexstrasza = gl_texture_2d("../opengl_utility/textures/alexstrasza.jpg");
        
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    v5 vertex_buffer[] = {
        {0.25, 0.5, 0, 1, 1},
        {0.25, -0.5, 0, 1, 0},
        {-0.25, -0.5, 0, 0, 0},
        {-0.25, 0.5, 0, 0, 1}
    };
    
    glBindVertexArray(app_state->gl_utility_context.rect_3f2f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, app_state->gl_utility_context.rect_3f2f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app_state->gl_utility_context.rect_3f2f.ebo);
    glUseProgram(app_state->texture_mix_program);
    int tex0_location = gl_get_uniform_location(app_state->texture_mix_program, "texture0");
    int tex1_location = gl_get_uniform_location(app_state->texture_mix_program, "texture1");
    glUniform1i(tex0_location, 0);
    glUniform1i(tex1_location, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.wall.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.awesome_face.id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}