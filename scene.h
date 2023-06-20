/* date = June 20th 2023 3:51 pm */

#ifndef SCENE_H
#define SCENE_H
struct Scene {
    Entity player;
    
    int world_object_count;
    Entity objects[MAX_SCENE_OBJECTS];
    
    Editor_State editor;
    Orbit_Camera camera; // game play camera
};


enum Resource_Type {
    Mesh,
    Texture,
};

struct Scene_Resource {
    int id;
    Resource_Type type;
    OS_Max_Path_String path;
};
#endif //SCENE_H
