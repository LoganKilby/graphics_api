/* date = June 20th 2023 3:55 pm */

#ifndef EDITOR_H
#define EDITOR_H

struct Editor_State {
    v4 x_axis_color;
    v4 y_axis_color;
    v4 z_axis_color;
    
    b32 loaded_from_disk;
    OS_Max_Path_String active_scene_path;
    
    Orbit_Camera camera;
    v3 camera_target;
    b32 editing;
    b32 entity_viewer;
};

#endif //EDITOR_H
