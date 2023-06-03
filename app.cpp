// NOTE(lmk): Some game behavior can be determined by simply the polled state, like if the left mouse button is down right now.
// Other behavior depends on when exactly the event happened. That's what this function is for
void process_game_input_events(Application_State *app_state, Input_Event_List *list) {
    Input_Event event;
    while(get_next_input_event(list, &event)) {
        switch(event.device) {
            case Mouse: {
                if(event.key == GLFW_MOUSE_BUTTON_LEFT) {
                    if(event.action == GLFW_PRESS) {
                        //glfwSetInputMode(Platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    } else {
                        //glfwSetInputMode(Platform.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                }
                
            } break;
            
            
            case Keyboard: {
                
            } break;
            
            default: {
                assert(0);
            } break;
        }
    }
}


void update_player_pos(v3 *player_pos, Basis *player_basis, f32 speed, Orbit_Camera *camera) {
    v3 move = {};
    
    if(is_key_pressed(GLFW_KEY_W) || (is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT) && is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)))
        move += player_basis->front;
    if(is_key_pressed(GLFW_KEY_S))
        move -= player_basis->front;
    if(is_key_pressed(GLFW_KEY_A) || is_key_pressed(GLFW_KEY_Q))
        move -= player_basis->right;
    if(is_key_pressed(GLFW_KEY_D) || is_key_pressed(GLFW_KEY_E))
        move += player_basis->right;
    
    if(!zero_vector(move)) {
        *player_pos += normalize(move) * speed * Platform.delta_time;
    }
}


void update_active_camera(Application_State *app_state) {
    switch(app_state->active_camera_type) {
        case Fly: {
            if(!zero_vector(Platform.input_state.mouse_diff)) 
                rotate_fly_camera(&app_state->fly_camera, Platform.input_state.mouse_diff);
            
        } break;
        
        case Orbit: {
            if(!zero_vector(Platform.input_state.mouse_diff)) 
                rotate_orbit_camera(&app_state->orbit_camera, Platform.input_state.mouse_diff);
            
            if(Platform.input_state.mouse_scroll_delta) 
                zoom_orbit_camera(&app_state->orbit_camera, Platform.input_state.mouse_scroll_delta);
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
        glEnable(GL_MULTISAMPLE);
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
        basis_from_front(&app_state->player_basis, v3(0, 0, -1));
        
        
        //Spherical_Coordinates orbit_pos = orbit_camera_spherical_position(app_state->fly_camera.position, app_state->player_pos);
        attach_orbit_camera(&app_state->orbit_camera, app_state->player_pos, app_state->player_basis.front, 10);
        app_state->active_camera_type = Orbit;
        
        //
        // Editor
        //
        app_state->editor.x_axis_color = v4(0, 0, 1, 1);
        app_state->editor.y_axis_color = v4(0, 1, 0, 1);
        app_state->editor.z_axis_color = v4(1, 0, 0, 1);
        
        app_state->initialized = true;
    }
    
    scratch_arena.allocated = 0;
    
    // Update player orientation
    if (is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        v3 camera_pos = orbit_camera_eye(&app_state->orbit_camera, app_state->player_pos);
        v3 camera_front = normalize(app_state->player_pos - camera_pos);
        
        // TODO(lmk): This won't work if the player should move about the y axis!!
        v3 new_player_front = v3(camera_front.x, 0, camera_front.z);
        basis_from_front(&app_state->player_basis, new_player_front);
    }
    
    process_game_input_events(app_state, &Platform.input_state.event_list);
    update_player_pos(&app_state->player_pos, &app_state->player_basis, app_state->player_speed, &app_state->orbit_camera);
    
    // if a ui window is hovered by the mouse, don't let mouse interact with the camera
    if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if(Platform.input_state.mouse_scroll_delta)
            zoom_orbit_camera(&app_state->orbit_camera, Platform.input_state.mouse_scroll_delta);
        
        if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT) || is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if(!zero_vector(Platform.input_state.mouse_diff)) 
                rotate_orbit_camera(&app_state->orbit_camera, Platform.input_state.mouse_diff);
        }
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //
    // Render game
    //
    // TODO(lmk): Update projection only on viewport resize
    GL_Viewport viewport;
    glGetIntegerv(GL_VIEWPORT, (int *)&viewport);
    mat4 projection = perspective(radians(45.0f), (f32)viewport.width / (f32)viewport.height, 0.1f, 100.0f);
    mat4 view = get_active_camera_transform(app_state);
    app_state->gl_utility_context.projection_3d = projection;
    app_state->gl_utility_context.view_3d = view;
    learnoepngl_camera(app_state, &projection, &view);
    gl_cube(app_state->player_pos, &app_state->player_basis, v4(1, 1, 1, 1));
    gl_basis(app_state->player_pos, &app_state->player_basis);
    
    //
    // Render editor
    // 
    ImGui_BeginFrame();
    bool show_demo_window;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui_EndFrame();
}
