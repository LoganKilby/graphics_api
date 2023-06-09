/* date = May 3rd 2023 8:46 pm */

#ifndef CORE_H
#define CORE_H

#ifdef DEBUG
#define gl_assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
#define gl_assert(expression) fprintf(stderr, "GL Assert: %s(%d\n", __FUNCTION__, __LINE__);
#endif

#include "vector_math.h"


struct GL_Array_Buffer {
    GLuint vbo;
    GLuint vao;
};


struct GL_Element_Buffer {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
};


static void gl_array_buffer_2f2f(GL_Array_Buffer *o);
static void gl_array_buffer_2f2f1f(GL_Array_Buffer *o);
static void gl_element_buffer_3f(GL_Element_Buffer *o, GLuint *indices, int index_count);
static void gl_element_buffer_3f2f(GL_Element_Buffer *o, GLuint *indices, int index_count);
static void gl_array_buffer_3f(GL_Array_Buffer *o);


struct GL_Shape_Context {
    GLuint v3f;
};


struct GL_Viewport {
    int x;
    int y;
    int width;
    int height;
};

#include "texture.h"


struct GL_Utility_Context {
    GLuint initialized;
    
    GL_Array_Buffer array_buffer_v3f;
    
    GLuint program_static_color_v3f;
    GLint uniform_static_color_v3f_color;
    GLuint program_texture_v3f_uv2f;
    GLint uniform_texture_v3f_uv2f_texture0;
    
    GLuint program_transform_static_color_v3f;
    GLint uniform_transform_static_color_v3f_color;
    GLint uniform_transform_static_color_v3f_model;
    GLint uniform_transform_static_color_v3f_view;
    GLint uniform_transform_static_color_v3f_projection;
    
    GL_Texture2D wall;
    GL_Texture2D awesome_face;
    
    mat4 ortho_2d;
    mat4 projection_3d;
    mat4 view_3d;
};


// TODO(lmk): Cleanup
static GL_Utility_Context *gl_utility_context_ptr;


static void gl_get_viewport_dimensions(int *width, int *height) {
    GLint dim[4];
    glGetIntegerv(GL_VIEWPORT, dim);
    *width = dim[2];
    *height = dim[3];
}


#include "shapes.h"
#include "shader.h"


char *gl_get_error_string(GLenum err) {
    switch(err) {
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_CONTEXT_LOST: return "GL_CONTEXT_LOST";
        case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE";
    }
    
    return "\0";
}


static void gl_utility_init(GL_Utility_Context *context) {
    memset(context, 0, sizeof(GL_Utility_Context));
    
    stbi_set_flip_vertically_on_load(true);
    
    gl_array_buffer_3f(&context->array_buffer_v3f);
    
    GL_Compiled_Shaders sh = {};
    
    sh.vert = gl_compile_shader(global_gl_shape_vert, 0, GL_VERTEX_SHADER);
    sh.frag = gl_compile_shader(global_gl_shape_frag, 0, GL_FRAGMENT_SHADER);
    context->program_static_color_v3f = gl_link_program(&sh);
    context->uniform_static_color_v3f_color = gl_get_uniform_location(context->program_static_color_v3f, "u_color");
    glDeleteShader(sh.vert);
    glDeleteShader(sh.frag);
    sh = {};
    
    sh.vert = gl_compile_shader(global_gl_texture_vert, 0, GL_VERTEX_SHADER);
    sh.frag = gl_compile_shader(global_gl_texture_frag, 0, GL_FRAGMENT_SHADER);
    context->program_texture_v3f_uv2f = gl_link_program(&sh);
    glDeleteShader(sh.vert);
    glDeleteShader(sh.frag);
    sh = {};
    
    sh.vert = gl_compile_shader(global_gl_mvp_static_color_vert, 0, GL_VERTEX_SHADER);
    sh.frag = gl_compile_shader(global_gl_shape_frag, 0, GL_FRAGMENT_SHADER);
    context->program_transform_static_color_v3f = gl_link_program(&sh);
    context->uniform_transform_static_color_v3f_color = gl_get_uniform_location(context->program_transform_static_color_v3f, "u_color");
    context->uniform_transform_static_color_v3f_view = gl_get_uniform_location(context->program_transform_static_color_v3f, "u_view");
    context->uniform_transform_static_color_v3f_model = gl_get_uniform_location(context->program_transform_static_color_v3f, "u_model");
    context->uniform_transform_static_color_v3f_projection = gl_get_uniform_location(context->program_transform_static_color_v3f, "u_projection");
    glDeleteShader(sh.vert);
    glDeleteShader(sh.frag);
    sh = {};
    
    context->awesome_face = gl_texture_2d("opengl_utility/textures/awesomeface.png");
    context->wall = gl_texture_2d("opengl_utility/textures/wall.jpg");
    
    int width, height;
    gl_get_viewport_dimensions(&width, &height);
    context->ortho_2d = glm::ortho(0.0f, (f32)width, (f32)height, 0.0f, 0.1f, 100.0f);
    
    context->initialized = 1;
    gl_utility_context_ptr = context;
}


static void gl_array_buffer_3f3f(GLuint *vao_out, GLuint *vbo_out) {
    GLuint vao, vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    *vao_out = vao;
    *vbo_out = vbo;
}


static void gl_array_buffer_3f2f(GL_Array_Buffer *o) {
    glGenBuffers(1, &o->vbo);
    glGenVertexArrays(1, &o->vao);
    
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


static void gl_array_buffer_2f2f(GL_Array_Buffer *o) {
    glGenVertexArrays(1, &o->vao);
    glGenBuffers(1, &o->vbo);
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


static void gl_array_buffer_5f(GL_Array_Buffer *o) {
    glGenVertexArrays(1, &o->vao);
    glGenBuffers(1, &o->vbo);
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


static void gl_array_buffer_3f(GL_Array_Buffer *o) {
    glGenBuffers(1, &o->vbo);
    glGenVertexArrays(1, &o->vao);
    
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}


static void gl_element_buffer_3f2f(GL_Element_Buffer *o, GLuint *indices, int index_count) {
    glGenBuffers(1, &o->ebo);
    glGenBuffers(1, &o->vbo);
    glGenVertexArrays(1, &o->vao);
    
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


static void gl_element_buffer_3f(GL_Element_Buffer *o, GLuint *indices, int index_count) {
    glGenBuffers(1, &o->ebo);
    glGenBuffers(1, &o->vbo);
    glGenVertexArrays(1, &o->vao);
    
    glBindVertexArray(o->vao);
    glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

#endif //CORE_H
