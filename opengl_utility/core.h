/* date = May 3rd 2023 8:46 pm */

#ifndef CORE_H
#define CORE_H

struct Shape_VBO_Context {
    GLuint quad;
};

struct GL_Utility_Context {
    Shape_VBO_Context shapes;
} static global_gl_utility_context;

#include "shader.h"
#include "shapes.h"

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

static void gl_utility_initialize() {
    global_gl_utility_context = {};
    
    glGenBuffers(sizeof(Shape_VBO_Context) / sizeof(GLuint), (GLuint *)&global_gl_utility_context.shapes);
}

#endif //CORE_H
