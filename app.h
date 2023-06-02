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


struct Application_State {
    b32 initialized;
    b32 edit_mode;
    
    GL_Utility_Context gl_utility_context;
    Editor_State editor;
    
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    
    GL_Array_Buffer v3f_uv2f;
    GL_Texture2D alexstrasza;
    
    Camera_Type active_camera_type;
    Fly_Camera fly_camera;
    Orbit_Camera orbit_camera;
    
    v3 player_pos;
    f32 player_speed;
    
    Entity player;
};

#endif //APP_H
