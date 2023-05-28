void update_player_pos(v3 *player_pos, f32 speed) {
    v3 move = {};
    
    if(glfwGetKey(Platform.window, GLFW_KEY_W) == GLFW_PRESS)
        --move.z;
    if(glfwGetKey(Platform.window, GLFW_KEY_S) == GLFW_PRESS)
        ++move.z;
    if(glfwGetKey(Platform.window, GLFW_KEY_A) == GLFW_PRESS)
        --move.x;
    if(glfwGetKey(Platform.window, GLFW_KEY_D) == GLFW_PRESS)
        ++move.x;
    
    if(!zero_vector(move))
        *player_pos += normalize(move) * speed * Platform.delta_time;
}

void update_active_camera(Application_State *app_state) {
    switch(app_state->active_camera_type) {
        case Fly: {
            if(!zero_vector(Platform.mouse_diff)) 
                rotate_fly_camera(&app_state->fly_camera, Platform.mouse_diff);
            
        } break;
        
        case Orbit: {
            if(!zero_vector(Platform.mouse_diff)) 
                rotate_orbit_camera(&app_state->orbit_camera, Platform.mouse_diff);
            
            if(Platform.mouse_scroll_delta) 
                zoom_orbit_camera(&app_state->orbit_camera, Platform.mouse_scroll_delta);
        } break;
        
        default: {
            assert(0); // No active camera
        }; 
    }
}


mat4 get_active_camera_transform(Application_State *app_state) {
    mat4 result;
    switch(app_state->active_camera_type) {
        case Fly: {
            result = lookAt_fly_camera(&app_state->fly_camera);
        } break;
        
        case Orbit: {
            result = lookAt_orbit_camera(&app_state->orbit_camera, app_state->player_pos);
        } break;
        
        default: {
            result = mat4(1.0f);
            assert(0); // No active camera
        };
    }
    
    return result;
}


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


void update_and_render(Memory_Arena *platform_memory) {
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
        
        gl_array_buffer_3f2f(&app_state->v3f_uv2f);
        app_state->alexstrasza = gl_texture_2d("opengl_utility/textures/alexstrasza.jpg");
        
        glClearColor(0, 0, 0, 0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        //
        // Camera
        //
        app_state->fly_camera.position = DEFAULT_CAMERA_POS;
        app_state->fly_camera.basis.front = v3(0, 0, -1);
        basis_from_front(&app_state->fly_camera.basis, app_state->fly_camera.basis.front);
        app_state->fly_camera.yaw = -90.0f;
        app_state->fly_camera.look_speed = DEFAULT_FLY_CAMERA_LOOK_SPEED;
        app_state->fly_camera.move_speed = DEFAULT_FLY_CAMERA_MOVE_SPEED;
        app_state->orbit_camera.look_speed = DEFAULT_ORBIT_CAMERA_LOOK_SPEED;
        app_state->orbit_camera.zoom_speed = DEFAULT_ORBIT_CAMERA_ZOOM_SPEED;
        
        app_state->player_pos = v3(0, 0, 0);
        app_state->player_speed = DEFAULT_PLAYER_SPEED;
        v3 player_front = v3(0, 0, -1);
        //Spherical_Coordinates orbit_pos = orbit_camera_spherical_position(app_state->fly_camera.position, app_state->player_pos);
        attach_orbit_camera(&app_state->orbit_camera, app_state->player_pos, player_front, 10);
        app_state->active_camera_type = Orbit;
        
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    int screen_width = 800;
    int screen_width_half = screen_width / 2;
    int screen_height = 600;
    int screen_height_half = screen_height / 2;
    
    update_player_pos(&app_state->player_pos, app_state->player_speed);
    update_active_camera(app_state);
    
    mat4 view = get_active_camera_transform(app_state);
    mat4 projection = perspective(radians(45.0f), (f32)screen_width / (f32)screen_height, 0.1f, 100.0f);
    learnoepngl_camera(app_state, &projection, &view);
    gl_cube(app_state->player_pos, v4(1, 0, 0, 1), &projection, &view);
}
