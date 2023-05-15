void update_and_render(Memory_Arena *platform_memory, Platform_Stuff *platform) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    assert(sizeof(Application_State) < APP_MEMORY_SIZE); // NOTE(lmk): Increase app memory size
    
    gl_utility_context_ptr = &app_state->gl_utility_context;
    
    if(!app_state->initialized) {
        scratch_arena = create_arena_local((u8 *)platform_memory->base_address + sizeof(Application_State), APP_MEMORY_SIZE - sizeof(Application_State));
        
        gl_utility_init(&app_state->gl_utility_context);
        
        gl_vertex_buffer_3f3f(&app_state->test_vao, &app_state->test_vbo);
        
        GL_Utility_Compiled_Shaders sh = {};
        
        u32 src_size;
        char *vert_source = (char *)os_read_entire_file("shaders/vertex3f3f.vert", &src_size);
        sh.vert = gl_compile_shader(vert_source, src_size, GL_VERTEX_SHADER);
        char *frag_source = (char *)os_read_entire_file("shaders/vertex3f3f.frag", &src_size);
        sh.frag = gl_compile_shader(frag_source, src_size, GL_FRAGMENT_SHADER);
        app_state->test_program = gl_link_program(&sh);
        
        vert_source = (char *)os_read_entire_file("shaders/v3f_uv2f.vert", &src_size);
        sh.vert = gl_compile_shader(vert_source, src_size, GL_VERTEX_SHADER);
        frag_source = (char *)os_read_entire_file("shaders/texture_mix_v3f_uv2f.frag", &src_size);
        sh.frag = gl_compile_shader(frag_source, src_size, GL_FRAGMENT_SHADER);
        app_state->texture_mix_program = gl_link_program(&sh);
        
        glClearColor(0, 0, 0, 0);
        
        gl_vertex_buffer_3f2f(&app_state->v3f_uv2f);
        
        app_state->alexstrasza = gl_texture_2d("opengl_utility/textures/alexstrasza.jpg");
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        app_state->camera_pos = v3(0, 0, 3);
        
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    int screen_width = 800;
    int screen_width_half = screen_width / 2;
    int screen_height = 600;
    int screen_height_half = screen_height / 2;
    
    v5 vertex_buffer[] = {
        {0.5, 0.5, 0, 1, 1},
        {0.5, -0.5, 0, 1, 0},
        {-0.5, -0.5, 0, 0, 0},
        {-0.5, 0.5, 0, 0, 1}
    };
    
    v3 world_up = v3(0, 1, 0);
    v3 camera_up = v3(0, 1, 0);
    v3 camera_front = v3(0, 0, -1);
    v3 camera_move = v3(0);
    if(glfwGetKey(platform->window, GLFW_KEY_W) == GLFW_PRESS)
        camera_move += camera_front;
    if(glfwGetKey(platform->window, GLFW_KEY_S) == GLFW_PRESS)
        camera_move -= camera_front;
    if(glfwGetKey(platform->window, GLFW_KEY_A) == GLFW_PRESS)
        camera_move -= normalize(cross(camera_front, camera_up));
    if(glfwGetKey(platform->window, GLFW_KEY_D) == GLFW_PRESS)
        camera_move += normalize(cross(camera_front, camera_up));
    
    if(!zero_vector(camera_move))
        app_state->camera_pos += normalize(camera_move) * CAMERA_SPEED * platform->delta_time;
    
    mat4 view = lookAt(app_state->camera_pos, app_state->camera_pos + camera_front, camera_up);
    mat4 model = glm::rotate(mat4(1.0f), radians(-55.0f), v3(1, 0, 0));
    mat4 projection = glm::perspective(radians(45.0f), (f32)screen_width / (f32)screen_height, 0.1f, 100.0f);
    
    glBindVertexArray(app_state->gl_utility_context.rect_3f2f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, app_state->gl_utility_context.rect_3f2f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app_state->gl_utility_context.rect_3f2f.ebo);
    glUseProgram(app_state->texture_mix_program);
    int tex0_location = gl_get_uniform_location(app_state->texture_mix_program, "texture0");
    int tex1_location = gl_get_uniform_location(app_state->texture_mix_program, "texture1");
    int transform_location = gl_get_uniform_location(app_state->texture_mix_program, "transform");
    glUniform1i(tex0_location, 0);
    glUniform1i(tex1_location, 1);
    mat4 transform = projection * view * model;
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, (f32 *)&transform);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.wall.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.awesome_face.id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
    glBindVertexArray(app_state->v3f_uv2f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, app_state->v3f_uv2f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v3f_uv2f), cube_v3f_uv2f, GL_STATIC_DRAW);
    for(int i = 0; i < countof(cubePositions); ++i) {
        model = mat4(1.0f);
        model = translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = rotate(model, radians(angle), v3(1, 0.3, 0.5));
        transform = projection * view * model;
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, (f32 *)&transform);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}