Entity *get_player_entity(Scene *scene) {
    // NOTE(lmk): Defining the player entity as always index 0
    // This helps when serializing/deserializing because I don't have to treat it any differently from other entity objects
    return &scene->entities[0];
}

// NOTE(lmk): processes events trigger immediately by input
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
                } else if (event.key == GLFW_MOUSE_BUTTON_RIGHT) {
                    if(event.action == GLFW_PRESS) {
                        
                    }
                }
            } break;
            
            case Keyboard: {
                switch(event.key) {
                    case GLFW_KEY_GRAVE_ACCENT: {
                        if(event.action == GLFW_PRESS) {
                            app_state->editor.editing ^= 1;
                            app_state->editor.camera = app_state->scene.camera;
                            app_state->editor.camera_target = app_state->scene.entities[0].position;
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
                            app_state->editor.editing ^= 1;
                        }
                    } break;
                    
                    case GLFW_KEY_S: {
                        if((event.action == GLFW_PRESS) && (event.mods & GLFW_MOD_CONTROL)) {
                            
                        }
                    } break;
                    
                    case GLFW_KEY_L: {
                        if((event.action == GLFW_PRESS) && (event.mods & GLFW_MOD_CONTROL)) {
                            
                        }
                    } break;
                    
                    
                    case GLFW_KEY_O: {
                        if((event.action == GLFW_PRESS) && (event.mods & GLFW_MOD_CONTROL)) {
                            
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
    if(app_state->editor.editing) 
        camera = &app_state->editor.camera;
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
    reset_arena(&transient_arena);
    
    Application_State *app_state = (Application_State *)platform_memory;
    
    gl_utility_context_ptr = &app_state->gl_utility_context;
    Renderer *renderer = &app_state->renderer;
    
    if(!app_state->initialized) {
        //
        // Renderer
        //
        gl_utility_init(&app_state->gl_utility_context);
        gl_array_buffer_3f3f(&app_state->test_vao, &app_state->test_vbo);
        GL_Compiled_Shaders sh = {};
        sh.vert = gl_compile_shader("shaders/vertex3f3f.vert", GL_VERTEX_SHADER);
        sh.frag = gl_compile_shader("shaders/vertex3f3f.frag", GL_FRAGMENT_SHADER);
        app_state->test_program = gl_link_program(&sh);
        sh.vert = gl_compile_shader("shaders/v3f_uv2f.vert", GL_VERTEX_SHADER);
        sh.frag = gl_compile_shader("shaders/texture_mix_v3f_uv2f.frag", GL_FRAGMENT_SHADER);
        app_state->texture_mix_program = gl_link_program(&sh);
        gl_array_buffer_3f2f(&app_state->v3f_uv2f);
        app_state->alexstrasza = gl_texture_2d("opengl_utility/textures/alexstrasza.jpg");
        glClearColor(0.1, 0.1, 0.1, 0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        app_state->renderer.create();
        
        Triangle_Mesh m = {};
        m.load("geo/fox.obj");
        
        // Fonts
        load_font(&app_state->consola, "fonts/consola.ttf", 48);
        v2 notify_origin(renderer->viewport.width / 2, renderer->viewport.height - (renderer->viewport.height / 8));
        v3 notify_color = v3(1, 1, 1);
        app_state->notifier.create(&renderer->font_renderer, &app_state->consola, notify_origin, notify_color, 1);
        
        if(Platform.argc > 1) {
            
        }
        
        // NOTE(lmk): entity 0 is always the player
        app_state->scene.player = &app_state->scene.entities[0];
        app_state->scene.player->position = v3(0, 0, 0);
        app_state->scene.player->scale = v3(1, 1, 1);
        basis_from_front(&app_state->scene.player->basis, v3(0, 0, -1));
        app_state->scene.entity_count++;
        
        app_state->scene.player->position = v3(0.11144, 1.23445, 1234.1234);
        
        //
        // Camera
        //
        Orbit_Camera game_camera = {};
        game_camera.look_speed = DEFAULT_ORBIT_CAMERA_LOOK_SPEED;
        game_camera.zoom_speed = DEFAULT_ORBIT_CAMERA_ZOOM_SPEED;
        game_camera.position.radius = 10;
        attach_orbit_camera(&game_camera, app_state->scene.player->position);
        v3 camera_pos = orbit_camera_eye(&game_camera);
        v3 front = normalize(game_camera.target - camera_pos);
        basis_from_front(&game_camera.basis, front);
        app_state->scene.camera = game_camera;
        
        //
        // Editor
        //
        app_state->editor.x_axis_color = v4(0, 0, 1, 1);
        app_state->editor.y_axis_color = v4(0, 1, 0, 1);
        app_state->editor.z_axis_color = v4(1, 0, 0, 1);
        app_state->editor.editing = 0;
        app_state->scene.camera.pan_speed = DEFAULT_ORBIT_CAMERA_PAN_SPEED;
        
        //
        //
        //
        // load_scene_data(&app_state->scene);
        save_scene(&app_state->scene, "scene0.scene");
        load_scene(&app_state->scene, "scene0.scene");
        
        app_state->initialized = true;
    }
    
    Editor_State *editor = &app_state->editor;
    Orbit_Camera *active_camera;
    
    if(editor->editing) {
        process_editor_input_events(app_state);
        
        active_camera = &editor->camera;
        
        // if a ui window is hovered by the mouse, don't let mouse interact with the camera
        if(!ImGuiUtil_WantCaptureMouse()) {
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
        update_player_pos(&app_state->scene.player->position, &app_state->scene.player->basis, DEFAULT_PLAYER_SPEED, active_camera);
        active_camera->target = app_state->scene.player->position;
        
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
            v3 new_player_front_xz = normalize(cross(UP, active_camera->basis.right));
            basis_from_front(&app_state->scene.player->basis, new_player_front_xz);
        }
    }
    
    app_state->notifier.update();
    
    //
    // Render game
    //
    // TODO(lmk): Update projection only on viewport resize
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    mat4 view = lookAt_orbit_camera(active_camera);
    
    app_state->gl_utility_context.projection_3d = renderer->projection_3d;
    app_state->gl_utility_context.view_3d = view;
    learnoepngl_camera(app_state, &renderer->projection_3d, &view);
    gl_cube(app_state->scene.player->position, &app_state->scene.player->basis, v4(1, 1, 1, 1));
    gl_basis(app_state->scene.player->position, &app_state->scene.player->basis);
    
    int frames_per_second = (int)(1000.0f / Platform.delta_time);
    char str[100] = {};
    sprintf(str, "FPS: %d", Platform.average_fps);
    
    v3 fps_color = v3(0, 1, 0);
    renderer->font_renderer.text(&app_state->consola, str, 25, 25, 1.0f, fps_color);
    renderer->render();
    
    //
    // Render editor
    if(editor->editing) {
        ImGui_BeginFrame();
        bool show_demo_window;
        ImGui::ShowDemoWindow(&show_demo_window);
        draw_editor(app_state);
        ImGui_EndFrame();
    }
}
