void learnoepngl_camera(Application_State *app_state, mat4 *projection, mat4 *view) {
    glUseProgram(app_state->texture_mix_program);
    int proj_transform_location = gl_get_uniform_location(app_state->texture_mix_program, "u_projection");
    int view_transform_location = gl_get_uniform_location(app_state->texture_mix_program, "u_view");
    int model_transform_location = gl_get_uniform_location(app_state->texture_mix_program, "u_model");
    glUniformMatrix4fv(proj_transform_location, 1, GL_FALSE, (f32 *)projection);
    glUniformMatrix4fv(view_transform_location, 1, GL_FALSE, (f32 *)view);
    int tex0_location = gl_get_uniform_location(app_state->texture_mix_program, "u_texture0");
    int tex1_location = gl_get_uniform_location(app_state->texture_mix_program, "u_texture1");
    glUniform1i(tex0_location, 0);
    glUniform1i(tex1_location, 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.wall.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, app_state->gl_utility_context.awesome_face.id);
    
    //
    // Draw Cubes
    //
    size_t cube_buffer_size, cube_vertex_size;
    f32 *cube_vertices = get_vertices_cube(&cube_buffer_size, &cube_vertex_size);
    
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
    glBufferData(GL_ARRAY_BUFFER, cube_buffer_size, cube_vertices, GL_STATIC_DRAW);
    mat4 model;
    for(int i = 0; i < countof(cubePositions); ++i) {
        model = mat4(1.0f);
        model = translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = rotate(model, radians(angle), v3(1, 0.3, 0.5));
        glUniformMatrix4fv(model_transform_location, 1, GL_FALSE, (f32 *)&model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


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
        
        gl_array_buffer_3f2f(&app_state->v3f_uv2f);
        
        app_state->alexstrasza = gl_texture_2d("opengl_utility/textures/alexstrasza.jpg");
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        app_state->camera.position = v3(0, 0, 3);
        app_state->camera.basis.front = v3(0, 0, -1);
        app_state->yaw = -90.0f;
        
        // NOTE(lmk): follow camera... camera should rotate around player up vector?
        app_state->player_pos = v3(1, 0, 0);
        
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    int screen_width = 800;
    int screen_width_half = screen_width / 2;
    int screen_height = 600;
    int screen_height_half = screen_height / 2;
    
    v3 world_up = v3(0, 1, 0);
    
    if(!zero_vector(platform->mouse_diff)) {
        app_state->yaw += platform->mouse_diff.x * MOUSE_SENSITIVITY;
        app_state->pitch += platform->mouse_diff.y * MOUSE_SENSITIVITY;
        app_state->pitch = clamp(app_state->pitch, -89.0f, 89.0f);
        
        v3 new_camera_front = app_state->camera.basis.front;
        new_camera_front.x = cos(radians(app_state->yaw)) * cos(radians(app_state->pitch));
        new_camera_front.y = sin(radians(app_state->pitch));
        new_camera_front.z = sin(radians(app_state->yaw)) * cos(radians(app_state->pitch));
        new_camera_front = normalize(new_camera_front);
        
        basis_from_front(&app_state->camera.basis, new_camera_front);
    }
    
    v3 camera_move = v3(0);
    if(glfwGetKey(platform->window, GLFW_KEY_W) == GLFW_PRESS)
        camera_move += app_state->camera.basis.front;
    if(glfwGetKey(platform->window, GLFW_KEY_S) == GLFW_PRESS)
        camera_move -= app_state->camera.basis.front;
    if(glfwGetKey(platform->window, GLFW_KEY_A) == GLFW_PRESS)
        camera_move -= normalize(cross(app_state->camera.basis.front, app_state->camera.basis.up));
    if(glfwGetKey(platform->window, GLFW_KEY_D) == GLFW_PRESS)
        camera_move += normalize(cross(app_state->camera.basis.front, app_state->camera.basis.up));
    
    if(!zero_vector(camera_move))
        app_state->camera.position += normalize(camera_move) * CAMERA_SPEED * platform->delta_time;
    
    mat4 view = lookAt(app_state->camera.position, app_state->camera.position + app_state->camera.basis.front, app_state->camera.basis.up);
    mat4 projection = perspective(radians(45.0f), (f32)screen_width / (f32)screen_height, 0.1f, 100.0f);
    
    learnoepngl_camera(app_state, &projection, &view);
    gl_cube(app_state->player_pos, v4(1, 0, 0, 1), &projection, &view);
}
