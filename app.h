/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#define APP_MEMORY_SIZE kilobytes(16)
#define WORLD_UP v3(0, 1, 0)

#define ImGui_BeginFrame() ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
#define ImGui_EndFrame() ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

struct Entity {
    v3 position;
    Basis basis;
};

struct Scene {
    Entity player;
    
    int world_object_count;
    Entity objects[MAX_SCENE_OBJECTS];
    
    Editor_State editor;
    Orbit_Camera camera; // game play camera
};


struct Application_State {
    b32 initialized;
    b32 edit_mode;
    
    GL_Utility_Context gl_utility_context;
    
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    u32 font_program;
    
    GL_Texture2D test_atlas;
    GL_Image smile1;
    GL_Image smile2;
    GL_Texture2D smile1t;
    GL_Texture2D smile2t;
    
    Rect smile1_rect, smile2_rect;
    
    GLS_Textured_Polygon textured_polygon_shader;
    
    GL_Texture_Rect texture_rect;
    
    GL_Array_Buffer v3f_uv2f;
    GL_Texture2D alexstrasza;
    
    Font font;
    
    Scene scene;
};


#endif //APP_H
