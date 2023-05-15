/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#include "opengl_utility/core.h"
#define APP_MEMORY_SIZE kilobytes(16)

#define CAMERA_SPEED 2.0f

struct Application_State {
    bool initialized;
    
    GL_Utility_Context gl_utility_context;
    
    u32 test_vao;
    u32 test_vbo;
    u32 test_program;
    u32 texture_mix_program;
    
    GL_Vertex_Buffer v3f_uv2f;
    GL_Texture2D alexstrasza;
    
    v3 camera_pos;
};

#endif //APP_H
