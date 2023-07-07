/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#define ImGui_BeginFrame() ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
#define ImGui_EndFrame() ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// NOTE(lmk): Add allocator macros here to track allocations
#define scene_alloc(size) scratch_alloc(&scene_storage, size); 
#define permanent_alloc(size) scratch_alloc(&permanent_storage, size);

#define CFILE_ALLOC transient_alloc
#include "../cfile/cfile.h"

#include "font.h"
#include "camera.h"
#include "scene.h"
#include "entity.h"
#include "asset.h"
#include "serialize.h"


struct Renderer {
    GL_Viewport viewport;
    mat4 projection_3d;
    mat4 projection_2d;
    Font_Renderer font_renderer;
    f32 near_plane;
    f32 far_plane;
    f32 fov;
    
    void render();
    void resize_viewport(int x, int y, int width, int height);
};


void Renderer::render() {
    font_renderer.render(&projection_2d);
}


void Renderer::resize_viewport(int x, int y, int width, int height) {
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    
    projection_2d = ortho((f32)viewport.x, (f32)viewport.width, (f32)viewport.y, (f32)viewport.height);
    projection_3d = perspective(fov, (f32)viewport.width / (f32)viewport.height, near_plane, far_plane);
}


// NOTE(lmk): Building an engine from a selection of components
struct Engine {
    b32 edit_mode;
    
    Editor_State editor;
    Msg_Notifier notifier;
    GL_Utility_Context gl_utility_context;
    
    Renderer renderer;
};


struct Application_State {
    b32 initialized;
    
    Engine engine;
    
    // permanent
    Font consola;
    Orbit_Camera editor_camera;
    OS_Max_Path_String scene_path;
    
    // per level
    Scene scene;
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    GL_Array_Buffer v3f_uv2f;
    Texture2D alexstrasza;
    
    v3 paper_pos;
    Basis paper_basis;
    
    v3 basket_pos;
    Basis basket_basis;
    
    v3 camera_pos_offset;
    
    // NOTE(lmk): I want to draw lots of little paper bundles that the player throws
    // 
    
    // base paper entity
    //GL_Vertex_Buffer paper;
    
    Game_Assets assets;
    
    // Memory arenas
    Memory_Arena transient_arena;
};

#endif //APP_H
