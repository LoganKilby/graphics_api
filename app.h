/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#define APP_MEMORY_SIZE kilobytes(16)
#define WORLD_UP v3(0, 1, 0)

struct Application_State {
    bool initialized;
    
    GL_Utility_Context gl_utility_context;
    
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    
    GL_Array_Buffer v3f_uv2f;
    GL_Texture2D alexstrasza;
    
    Camera_Orientation camera;
    Orbit_Camera_Orientation orbit_camera;
    
    f32 yaw;
    f32 pitch;
    
    v3 player_pos;
};

#endif //APP_H
