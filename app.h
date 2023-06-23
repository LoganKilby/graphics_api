/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#define ImGui_BeginFrame() ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
#define ImGui_EndFrame() ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#include "font.h"
#include "mesh.h"
#include "camera.h"
#include "scene.h"
#include "entity.h"

struct Renderer {
    GL_Viewport viewport;
    mat4 projection_3d;
    mat4 projection_2d;
    Font_Renderer font_renderer;
    f32 near_plane;
    f32 far_plane;
    f32 fov;
    
    void render();
    void create();
    void resize_viewport(int x, int y, int width, int height);
};


void Renderer::render() {
    font_renderer.render(&projection_2d);
}


void Renderer::create() {
    near_plane = NEAR_PLANE;
    far_plane = FAR_PLANE;
    fov = FOV;
    
    glGetIntegerv(GL_VIEWPORT, (int *)&viewport);
    projection_2d = ortho((f32)viewport.x, (f32)viewport.width, (f32)viewport.y, (f32)viewport.height);
    projection_3d = perspective(fov, (f32)viewport.width / (f32)viewport.height, near_plane, far_plane);
    
    font_renderer.create();
}


void Renderer::resize_viewport(int x, int y, int width, int height) {
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    
    projection_2d = ortho((f32)viewport.x, (f32)viewport.width, (f32)viewport.y, (f32)viewport.height);
    projection_3d = perspective(fov, (f32)viewport.width / (f32)viewport.height, near_plane, far_plane);
}


struct Application_State {
    b32 initialized;
    b32 edit_mode;
    
    GL_Utility_Context gl_utility_context;
    
    // learnopengl stuff
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    
    Renderer renderer;
    
    GL_Array_Buffer v3f_uv2f;
    Texture2D alexstrasza;
    
    Font consola;
    Msg_Notifier notifier;
    
    b32 scene_loaded_from_disk;
    OS_Max_Path_String scene_path;
    Scene scene;
};


#endif //APP_H
