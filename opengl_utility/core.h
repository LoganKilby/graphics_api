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
    
} static global_gl_utility_context;

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


static void gl_utility_init() {
    global_gl_utility_context = {};
    
    // Shapes
    glGenBuffers(sizeof(GL_Shape_Context) / sizeof(GLuint), (GLuint *)&global_gl_utility_context.shape_vbo);
    glGenVertexArrays(sizeof(GL_Shape_Context) / sizeof(GLuint), (GLuint *)&global_gl_utility_context.shape_vao);
    
    glBindVertexArray(global_gl_utility_context.shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, global_gl_utility_context.shape_vbo.v3f);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    // ... more shapes
    glBindVertexArray(0);
    
    GL_Utility_Compiled_Shaders sh = {};
    GLint vert_length = (GLint)strlen(global_gl_shape_vert);
    sh.vert = gl_compile_shader(global_gl_shape_vert, vert_length, GL_VERTEX_SHADER);
    GLint frag_length = (GLint)strlen(global_gl_shape_frag);
    sh.frag = gl_compile_shader(global_gl_shape_frag, frag_length, GL_FRAGMENT_SHADER);
    global_gl_utility_context.static_color_program = gl_link_program(&sh);
    global_gl_utility_context.static_color_uniform_color = gl_get_uniform_location(global_gl_utility_context.static_color_program, "uniform_fragment_color");
    
    global_gl_utility_context.initialized = 1;
}

static GLuint gl_create_vao_3f3f() {
    GLuint vao, vbo;
    
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    return vao;
}

#else
// NOTE(lmk): Since I'm using a global variable to store state about the utility, there shouldn't be separate global variables
// defined in the same translation unit. Ideally there should only be one instance of the global variable. This error
// only prevents multiple declarations within the same translation unit.
#error Opengl Utility 'core.h' most only be included once.
#endif //CORE_H
