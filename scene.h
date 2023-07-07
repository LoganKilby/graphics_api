/* date = June 20th 2023 3:51 pm */

#ifndef SCENE_H
#define SCENE_H


struct Entity {
    int resource_id;
    v3 position;
    v3 scale;
    Basis basis;
};


struct Editor_State {
    v4 x_axis_color;
    v4 y_axis_color;
    v4 z_axis_color;
    
    Orbit_Camera camera;
    v3 camera_target;
    b32 editing;
    b32 entity_viewer;
    
    b32 scene_loaded_from_disk;
    OS_Max_Path_String scene_path;
};


#define MAX_SCENE_ENTITIES 128 // totally arbitrary for now
struct Scene {
    int entity_count;
    Entity entities[MAX_SCENE_ENTITIES];
    Orbit_Camera camera; // game play camera
};


#endif //SCENE_H
