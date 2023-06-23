bool save_scene(Scene *scene, OS_Max_Path_String *save_path) {
    if(cfile_write(save_path->data, scene, sizeof(Scene))) {
        return true;
    } else {
        fail("Unable to save scene file");
        return false;
    }
}


bool save_scene_as(Scene *scene, OS_Max_Path_String *path_out) {
    if(os_get_save_file_name(path_out->data, sizeof(path_out->data))) {
        return save_scene(scene, path_out);
    }
    
    return false;
}


bool load_scene(Scene *scene, OS_Max_Path_String *path) {
    if(os_get_open_file_name(path->data, sizeof(path->data))) {
        size_t bytes_read;
        Scene *data = (Scene *)cfile_read(path->data, &bytes_read);
        
        if(data) {
            if(bytes_read == sizeof(Scene)) {
                memcpy(&scene, data, sizeof(Scene));
                free(data);
            } else {
                fail("Size of Scene file does not match compiled size");
            }
        } else {
            fail("Unable to open Scene file or out of memory");
        }
        
        return true;
    }
    
    return false;
}


// TODO(lmk): Do a scene file format that I can parse
void draw_editor(Application_State *state) {
    // show main menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("Scene")) {
            if(ImGui::MenuItem("New")) {
                
            }
            
            if(ImGui::MenuItem("Save", "ctrl+s")) {
                if(state->scene_loaded_from_disk) {
                    if(save_scene(&state->scene, &state->scene_path)) state->scene_loaded_from_disk = true;
                } else {
                    if(save_scene_as(&state->scene, &state->scene_path)) state->scene_loaded_from_disk = true;
                }
            }
            
            if(ImGui::MenuItem("Save As...")) save_scene_as(&state->scene, &state->scene_path);
            if(ImGui::MenuItem("Load", "ctrl+l")) load_scene(&state->scene, &state->scene_path);
            ImGui::EndMenu();
        }
        
        
        if(ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Objects", "ctrl+o");
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}