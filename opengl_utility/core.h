/* date = May 3rd 2023 8:46 pm */

#ifndef CORE_H
#define CORE_H

struct GL_Shape_Context {
    GLuint v3f;
};

struct GL_Utility_Context {
    GLuint initialized;
    GL_Shape_Context shape_vbo;
    GL_Shape_Context shape_vao;
    
    GLuint static_color_program;
    GLuint static_color_uniform_color;
    
};

static GL_Utility_Context *gl_utility_context_ptr;

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
    
    // Shapes
    glGenBuffers(sizeof(GL_Shape_Context) / sizeof(GLuint), (GLuint *)&context->shape_vbo);
    glGenVertexArrays(sizeof(GL_Shape_Context) / sizeof(GLuint), (GLuint *)&context->shape_vao);
    
    glBindVertexArray(context->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, context->shape_vbo.v3f);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    // ... more shapes
    glBindVertexArray(0);
    
    GL_Utility_Compiled_Shaders sh = {};
    GLint vert_length = (GLint)strlen(global_gl_shape_vert);
    sh.vert = gl_compile_shader(global_gl_shape_vert, vert_length, GL_VERTEX_SHADER);
    GLint frag_length = (GLint)strlen(global_gl_shape_frag);
    sh.frag = gl_compile_shader(global_gl_shape_frag, frag_length, GL_FRAGMENT_SHADER);
    context->static_color_program = gl_link_program(&sh);
    context->static_color_uniform_color = gl_get_uniform_location(context->static_color_program, "uniform_fragment_color");
    
    context->initialized = 1;
    gl_utility_context_ptr = context;
}

static void gl_vertex_buffer_3f3f(GLuint *vao_out, GLuint *vbo_out) {
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

#endif //CORE_H
