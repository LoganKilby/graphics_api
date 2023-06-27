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
    create_blob(&blob);
    
    serialize_entity(&blob, scene->entities, scene->entity_count);
    serialize_orbit_camera(&blob, &scene->camera);
    
    S_Entity *se = (S_Entity *)((u8 *)blob.memory + blob.indices[1].value_offset);
    
    
    if(!cfile_write(path, blob.memory, blob.size)) {
        fail("Unable to save scene file");
        return false;
    }
    
    size_t s = sizeof(Blob_Header) + (sizeof(Blob_Index) * 3) + sizeof(S_Entity) + sizeof(S_Orbit_Camera) + BLOB_PADDING;
    
    Blob_Context blob_copy = {};
    load_blob(&blob_copy, path);
    
    free(blob.memory);
    return true;
}


bool save_scene_as(Scene *scene, OS_Max_Path_String *path_str_out) {
    if(os_get_save_file_name(path_str_out->data, sizeof(path_str_out->data))) {
        return save_scene(scene, path_str_out->data);
    }
    
    return false;
}



// TODO(lmk): This doesn't belong in this file
void draw_editor(Application_State *state) {
    // show main menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("Scene")) {
            if(ImGui::MenuItem("New")) {
                
            }
            
            if(ImGui::MenuItem("Save", "ctrl+s")) {
                if(state->scene_loaded_from_disk) {
                    if(save_scene(&state->scene, state->scene_path.data)) state->scene_loaded_from_disk = true;
                } else {
                    if(save_scene_as(&state->scene, &state->scene_path)) state->scene_loaded_from_disk = true;
                }
            }
            
            if(ImGui::MenuItem("Save As...")) save_scene_as(&state->scene, &state->scene_path);
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