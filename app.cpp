static GLuint gl_compile_shader(char *file_path, GLenum type) {
    u32 src_length;
    char *src = (char *)os_read_entire_file(file_path, &src_length);
    GLuint result = gl_compile_shader(src, src_length, type);
    free(src);
    return result;
}


void save_scene_data(Scene *scene) {
    cfile_write(DEBUG_SCENE_NAME, scene, sizeof(Scene));
}


void load_scene_data(Scene *scene_dest) {
    size_t bytes_read;
    Scene *data = (Scene *)cfile_read(DEBUG_SCENE_NAME, &bytes_read);
    assert(bytes_read == sizeof(Scene));
    memcpy(scene_dest, data, sizeof(Scene));
    free(data);
}


// NOTE(lmk): Some game behavior can be determined by simply the polled state, like if the left mouse button is down right now.
// Other behavior depends on when exactly the event happened. That's what this function is for
void process_game_input_events(Application_State *app_state) {
    Input_Event event;
    while(get_next_input_event(&event)) {
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
                switch(event.key) {
                    case GLFW_KEY_GRAVE_ACCENT: {
                        if(event.action == GLFW_PRESS) {
                            app_state->scene.editor.editing ^= 1;
                            app_state->scene.editor.camera = app_state->scene.camera;
                            app_state->scene.editor.camera_target = app_state->scene.player.position;
                        }
                    } break;
                }
            } break;
            
            default: {
                assert(0);
            } break;
        }
    }
}


void process_editor_input_events(Application_State *app_state) {
    Input_Event event;
    while(get_next_input_event(&event)) {
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
                switch(event.key) {
                    case GLFW_KEY_GRAVE_ACCENT: {
                        if(event.action == GLFW_PRESS) {
                            app_state->scene.editor.editing ^= 1;
                        }
                    } break;
                    
                    case GLFW_KEY_S: {
                        if((event.action == GLFW_PRESS) && (event.mods == GLFW_KEY_LEFT_CONTROL)) {
                            save_scene_data(&app_state->scene);
                            // TODO(lmk): Put a message on the screen
                            printf("scene saved\n");
                        }
                    } break;
                    
                    case GLFW_KEY_Z: {
                        
                    } break;
                }
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


Orbit_Camera *get_active_camera(Application_State *app_state) {
    Orbit_Camera *camera;
    if(app_state->scene.editor.editing) 
        camera = &app_state->scene.editor.camera;
    else
        camera = &app_state->scene.camera;
    
    return camera;
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


void update_and_render(void *platform_memory) {
    Application_State *app_state = (Application_State *)platform_memory;
    
    gl_utility_context_ptr = &app_state->gl_utility_context;
    
    if(!app_state->initialized) {
        gl_utility_init(&app_state->gl_utility_context);
        gl_vertex_buffer_3f3f(&app_state->test_vao, &app_state->test_vbo);
        GL_Utility_Compiled_Shaders sh = {};
        
        sh.vert = gl_compile_shader("shaders/vertex3f3f.vert", GL_VERTEX_SHADER);
        sh.frag = gl_compile_shader("shaders/vertex3f3f.frag", GL_FRAGMENT_SHADER);
        app_state->test_program = gl_link_program(&sh);
        
        sh.vert = gl_compile_shader("shaders/v3f_uv2f.vert", GL_VERTEX_SHADER);
        sh.frag = gl_compile_shader("shaders/texture_mix_v3f_uv2f.frag", GL_FRAGMENT_SHADER);
        app_state->texture_mix_program = gl_link_program(&sh);
        
        sh.vert = gl_compile_shader("shaders/font_shader.vert", GL_VERTEX_SHADER);
        sh.frag = gl_compile_shader("shaders/font_shader.frag", GL_FRAGMENT_SHADER);
        app_state->font_program = gl_link_program(&sh);
        
        gl_array_buffer_3f2f(&app_state->v3f_uv2f);
        app_state->alexstrasza = gl_texture_2d("opengl_utility/textures/alexstrasza.jpg");
        
        glClearColor(0, 0, 0, 0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // FreeType
        //create_font_atlas("fonts/consola.ttf", 16);
        //create_font_atlas("fonts/consola.ttf", 48);
        load_ascii_textures(&app_state->font, "fonts/consola.ttf", 48);
        create_font_atlas("fonts/consola.ttf", 48);
        
        app_state->scene.player.position = v3(0, 0, 0);
        
        //
        // Camera
        //
        app_state->scene.camera.look_speed = DEFAULT_ORBIT_CAMERA_LOOK_SPEED;
        app_state->scene.camera.zoom_speed = DEFAULT_ORBIT_CAMERA_ZOOM_SPEED;
        basis_from_front(&app_state->scene.player.basis, v3(0, 0, -1));
        attach_orbit_camera(&app_state->scene.camera, app_state->scene.player.position, app_state->scene.player.basis.front, 10);
        
        //
        // Editor
        //
        app_state->scene.editor.x_axis_color = v4(0, 0, 1, 1);
        app_state->scene.editor.y_axis_color = v4(0, 1, 0, 1);
        app_state->scene.editor.z_axis_color = v4(1, 0, 0, 1);
        app_state->scene.editor.editing = 0;
        app_state->scene.camera.pan_speed = DEFAULT_ORBIT_CAMERA_PAN_SPEED;
        
        //load_scene_data(&app_state->scene);
        
        app_state->initialized = true;
    }
    
    Editor_State *editor = &app_state->scene.editor;
    Orbit_Camera *active_camera = get_active_camera(app_state);
    
    if(editor->editing) {
        process_editor_input_events(app_state);
        
        active_camera = &app_state->scene.editor.camera;
        
        // if a ui window is hovered by the mouse, don't let mouse interact with the camera
        if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
            if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) 
                rotate_orbit_camera(active_camera, Platform.input_state.mouse_diff);
            
            if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) 
                pan_orbit_camera(active_camera, Platform.input_state.mouse_diff);
            
            if(Platform.input_state.mouse_scroll_delta)
                zoom_orbit_camera(active_camera, Platform.input_state.mouse_scroll_delta);
        }
    } else {
        process_game_input_events(app_state);
        active_camera = &app_state->scene.camera;
        update_player_pos(&app_state->scene.player.position, &app_state->scene.player.basis, DEFAULT_PLAYER_SPEED, active_camera);
        active_camera->target = app_state->scene.player.position;
        
        // if a ui window is hovered by the mouse, don't let mouse interact with the camera
        if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
            if(Platform.input_state.mouse_scroll_delta)
                zoom_orbit_camera(active_camera, Platform.input_state.mouse_scroll_delta);
            
            if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT) || is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
                if(!zero_vector(Platform.input_state.mouse_diff)) {
                    rotate_orbit_camera(active_camera, Platform.input_state.mouse_diff);
                }
            }
        }
        
        // Update player orientation
        if (is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            v3 camera_pos = orbit_camera_eye(active_camera);
            
            // TODO(lmk): This won't work if the player should move about the y axis!!
            v3 new_player_front = v3(active_camera->basis.front.x, 0, active_camera->basis.front.z);
            basis_from_front(&app_state->scene.player.basis, new_player_front);
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
    mat4 view = lookAt_orbit_camera(active_camera);
    app_state->gl_utility_context.projection_3d = projection;
    app_state->gl_utility_context.view_3d = view;
    learnoepngl_camera(app_state, &projection, &view);
    gl_cube(app_state->scene.player.position, &app_state->scene.player.basis, v4(1, 1, 1, 1));
    gl_basis(app_state->scene.player.position, &app_state->scene.player.basis);
    
    glm::mat4 projection_2d = glm::ortho(0.0f, (float)viewport.width, 0.0f, (float)viewport.height);
    
    int frames_per_second = (int)(1000.0f / Platform.delta_time);
    char str[100] = {};
    sprintf(str, "FPS: %d", Platform.average_fps);
    render_font(&app_state->font, str, 25, 25, 1.0f, &projection_2d, app_state->font_program);
    
    //
    // Render editor
    // Is 'if' still a problem, or 'If'?
    ImGui_BeginFrame();
    bool show_demo_window;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui_EndFrame();
}
