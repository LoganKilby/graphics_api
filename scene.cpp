void load_scene(Scene *scene, char *path) {
    Blob_Context blob = {};
    if(load_blob(&blob, path)) {
        deserialize(&blob, scene);
        free(blob.memory);
    } else {
        fail("Unable to open Scene file or out of memory");
    }
}

bool save_scene(Scene *scene, char *path) {
    Blob_Context blob;
    
    size_t total_blob_size = 0;
    total_blob_size += (sizeof(S_Entity) * scene->entity_count);
    total_blob_size += sizeof(S_Orbit_Camera);
    
    int element_count = scene->entity_count + 1;
    create_blob(&blob, element_count, total_blob_size);
    serialize_entity(&blob, scene->entities, scene->entity_count);
    serialize_orbit_camera(&blob, &scene->camera);
    
    if(!cfile_write(path, blob.memory, blob.size)) {
        fail("Unable to save scene file");
        return false;
    }
    
    return true;
}


// TODO(lmk): This doesn't belong in this file
void draw_editor(Application_State *state) {
    // show main menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("Scene")) {
            if(ImGui::MenuItem("New"), "ctrl+n") {
                state->editor.scene_loaded_from_disk = false;
            }
            
            if(ImGui::MenuItem("Save", "ctrl+s")) {
                if(state->editor.scene_loaded_from_disk) {
                    save_scene(&state->scene, state->editor.scene_path.data);
                } else {
                    if(os_get_save_file_name(state->editor.scene_path.data, sizeof(state->editor.scene_path.data))) {
                        save_scene(&state->scene, state->editor.scene_path.data);
                        state->editor.scene_loaded_from_disk = true;
                    }
                }
            }
            
            if(ImGui::MenuItem("Save As...")) {
                if(os_get_save_file_name(state->editor.scene_path.data, sizeof(state->editor.scene_path.data))) {
                    save_scene(&state->scene, state->editor.scene_path.data);
                    state->editor.scene_loaded_from_disk = true;
                }
            }
            
            if(ImGui::MenuItem("Load", "ctrl+l"))  {
                OS_Max_Path_String path = {};
                os_get_open_file_name(path.data, sizeof(path.data));
                load_scene(&state->scene, path.data);
            }
            
            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Objects", "ctrl+o");
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}