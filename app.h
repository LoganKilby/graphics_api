/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#define ImGui_BeginFrame() ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
#define ImGui_EndFrame() ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// Memory arenas
global Memory_Arena transient_storage;
global Memory_Arena scene_storage;
global Memory_Arena permanent_storage;

// NOTE(lmk): Add allocator macros here to track allocations
#define transient_alloc(size) transient_alloc(&transient_storage, size); DEBUG_REGISTER_ALLOCATION(transient_storage, size)
#define scene_alloc(size) transient_alloc(&scene_storage, size); DEBUG_REGISTER_ALLOCATION(scene_storage, size)
#define permanent_alloc(size) transient_alloc(&permanent_storage, size); DEBUG_REGISTER_ALLOCATION(permanent_storage, size)

#define CFILE_ALLOC transient_alloc
#include "../cfile/cfile.h"

#include "font.h"
#include "mesh.h"
#include "camera.h"
#include "scene.h"
#include "entity.h"

#define BLOBALLOC transient_alloc
#define BLOBFREE transient_reset_last_alloc
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
    Renderer *create();
    void resize_viewport(int x, int y, int width, int height);
};


void Renderer::render() {
    font_renderer.render(&projection_2d);
}


Renderer *Renderer::create() {
    near_plane = NEAR_PLANE;
    far_plane = FAR_PLANE;
    fov = FOV;
    
    glGetIntegerv(GL_VIEWPORT, (int *)&viewport);
    projection_2d = ortho((f32)viewport.x, (f32)viewport.width, (f32)viewport.y, (f32)viewport.height);
    projection_3d = perspective(fov, (f32)viewport.width / (f32)viewport.height, near_plane, far_plane);
    
    font_renderer.create();
    
    return this;
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
    // permanent
    b32 initialized;
    b32 edit_mode;
    GL_Utility_Context gl_utility_context;
    Renderer renderer;
    Font consola;
    Msg_Notifier notifier;
    Orbit_Camera editor_camera;
    Editor_State editor;
    OS_Max_Path_String scene_path;
    
    // per level
    Scene scene;
    // learnopengl stuff
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    GL_Array_Buffer v3f_uv2f;
    Texture2D alexstrasza;
};


#endif //APP_H
