void create_renderer(Renderer *renderer) {
    renderer->near_plane = NEAR_PLANE;
    renderer->far_plane = FAR_PLANE;
    renderer->fov = FOV;
    
    glGetIntegerv(GL_VIEWPORT, (int *)&renderer->viewport);
    
    renderer->projection_2d = ortho((f32)renderer->viewport.x, (f32)renderer->viewport.width, (f32)renderer->viewport.y, (f32)renderer->viewport.height);
    
    renderer->projection_3d = perspective(renderer->fov, (f32)renderer->viewport.width / (f32)renderer->viewport.height, renderer->near_plane, renderer->far_plane);
    
    renderer->font_renderer.create();
}


void begin_frame(Application_State *state) {
    ImGui_BeginFrame();
    state->engine.notifier.update();
    state->transient_arena.allocated = 0;
}


void end_frame(Application_State *state) {
    ImGui_EndFrame();
}


void initialize_application_state(Memory *memory) {
    Application_State *app_state = (Application_State *)memory->base_address;
    
    //
    // Memory
    //
    u8 *transient_arena_base_address = (u8 *)app_state + sizeof(Application_State);
    app_state->transient_arena = create_arena_local(transient_arena_base_address, TRANSIENT_ARENA_SIZE);
    
    //
    // Renderer
    //
    gl_utility_init(&app_state->engine.gl_utility_context);
    
    // refactor
    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // refactor 
    
    Renderer *renderer = &app_state->engine.renderer;
    create_renderer(renderer);
    
    // remove
    //Triangle_Mesh m = {};
    //load_mesh("geo/fox.obj");
    // remove
    
    // Fonts
    load_font(&app_state->consola, "assets/fonts/consola.ttf", 48);
    
    v2 notify_origin(renderer->viewport.width / 2, renderer->viewport.height - (renderer->viewport.height / 8));
    v3 notify_color = v3(1, 1, 1);
    app_state->engine.notifier.create(&renderer->font_renderer, &app_state->consola, notify_origin, notify_color, 1);
    
    if(Platform.argc > 1) {
        
    }
    
    // TODO(lmk): Import from scene file
    app_state->paper_pos = v3(0, 0, 0);
    basis_from_front(&app_state->paper_basis, v3(0, 0, 1));
    
    app_state->basket_pos = v3(0, 0, 10);
    basis_from_front(&app_state->basket_basis, -app_state->paper_basis.front);
    
    //
    // Camera :: TODO(lmk): Import from scene file
    //
    Orbit_Camera game_camera = {};
    rotate_orbit_camera_polar(&game_camera, 0.25f);
    rotate_orbit_camera_azimuth(&game_camera, PI * 1.5f);
    game_camera.look_speed = DEFAULT_ORBIT_CAMERA_LOOK_SPEED;
    game_camera.zoom_speed = DEFAULT_ORBIT_CAMERA_ZOOM_SPEED;
    game_camera.position.radius = 10;
    attach_orbit_camera(&game_camera, app_state->paper_pos);
    v3 camera_pos = orbit_camera_eye(&game_camera);
    v3 front = normalize(game_camera.target - camera_pos);
    basis_from_front(&game_camera.basis, front);
    app_state->scene.camera = game_camera;
    
    //
    // Editor
    //
    Editor_State *editor = &app_state->engine.editor;
    editor->x_axis_color = v4(0, 0, 1, 1);
    editor->y_axis_color = v4(0, 1, 0, 1);
    editor->z_axis_color = v4(1, 0, 0, 1);
    app_state->scene.camera.pan_speed = DEFAULT_ORBIT_CAMERA_PAN_SPEED;
    
    //
    //
    //
    // load_scene_data(&app_state->scene);
    //save_scene(app_state, "scene0.scene");
    //load_scene(&app_state->scene, "scene0.scene");
    
    app_state->initialized = true;
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
                            Editor_State *editor = &app_state->engine.editor;
                            editor->editing ^= 1;
                            editor->camera = app_state->scene.camera;
                            editor->camera_target = app_state->scene.entities[0].position;
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
                            app_state->engine.editor.editing ^= 1;
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
    Editor_State *editor = &app_state->engine.editor;
    if(editor->editing) 
        camera = &editor->camera;
    else
        camera = &app_state->scene.camera;
    
    return camera;
}


void learnoepngl_camera(Application_State *app_state, mat4 *projection, mat4 *view) {
    GL_Utility_Context *gl_utility_context = &app_state->engine.gl_utility_context;
    
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
    glBindTexture(GL_TEXTURE_2D, gl_utility_context->wall.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gl_utility_context->awesome_face.id);
    
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


void update_and_render(Memory *platform_memory) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    if(!app_state->initialized) initialize_application_state(platform_memory);
    
    Renderer *renderer = &app_state->engine.renderer;
    Editor_State *editor = &app_state->engine.editor;
    Engine *engine = &app_state->engine;
    
    begin_frame(app_state);
    
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
        //update_player_pos(&app_state->scene.player->position, &app_state->scene.player->basis, DEFAULT_PLAYER_SPEED, active_camera);
        //active_camera->target = app_state->scene.player->position;
        
        if(Platform.input_state.mouse_scroll_delta)
            zoom_orbit_camera(active_camera, Platform.input_state.mouse_scroll_delta);
        
        if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT) || is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if(!zero_vector(Platform.input_state.mouse_diff)) {
                rotate_orbit_camera(active_camera, Platform.input_state.mouse_diff);
            }
        }
        
        // Update player orientation
        if (is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            // NOTE(lmk): align player front with camera front (xz)
            //v3 camera_pos = orbit_camera_eye(active_camera);
            //v3 new_player_front_xz = normalize(cross(UP, active_camera->basis.right));
            //basis_from_front(&app_state->scene.player->basis, new_player_front_xz);
        }
    }
    
    //
    // Render game
    //
    // TODO(lmk): Update projection only on viewport resize
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    mat4 view = lookAt_orbit_camera(active_camera);
    
    app_state->engine.gl_utility_context.projection_3d = renderer->projection_3d;
    engine->gl_utility_context.view_3d = view;
    
    gl_cube(app_state->paper_pos, &app_state->paper_basis, v4(1, 1, 1, 1));
    gl_cube(app_state->basket_pos, &app_state->basket_basis, v4(1, 1, 0, 1));
    
    gl_basis(app_state->basket_pos, &app_state->basket_basis);
    gl_basis(app_state->paper_pos, &app_state->paper_basis);
    
    int frames_per_second = (int)(1000.0f / Platform.delta_time);
    char str[100] = {};
    sprintf(str, "FPS: %d", Platform.average_fps);
    v3 fps_color = v3(0, 1, 0);
    renderer->font_renderer.text(&app_state->consola, str, 25, 25, 1.0f, fps_color, &app_state->transient_arena);
    renderer->render();
    
    //
    // Render editor
    if(editor->editing) {
        draw_editor(app_state);
    }
    
    printf("%f\n", active_camera->position.azimuth);
    
    end_frame(app_state);
}

