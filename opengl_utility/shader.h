/* date = May 1st 2023 7:17 pm */

#ifndef SHADER_H
#define SHADER_H

// NOTE(lmk): GLSL reference compiler uses the extensions .vert, .frag, .geom, .tesc, .tese, .comp

struct GL_Utility_Compiled_Shaders {
    GLuint vert;
    GLuint frag;
    GLuint geom;
    GLuint tesc;
    GLuint tese;
    GLuint comp;
};

static char *global_gl_utility_shape_vert =
#include "shaders/shape.vert"
;
static char *global_gl_utility_shape_frag =
#include "shaders/shape.frag"
;


#define INFO_LOG_LENGTH 1024


static GLuint gl_utility_compile_shader(char *source, GLint source_length, GLenum type) {
    GLuint result = 0;
    
    if(source) {
        result = glCreateShader(type);
        glShaderSource(result, 1, &source, &source_length);
        glCompileShader(result);
        
        GLint compile_status;
        glGetShaderiv(result, GL_COMPILE_STATUS, &compile_status);
        if(compile_status == GL_FALSE) {
            char info_log[INFO_LOG_LENGTH] = {};
            glGetShaderInfoLog(result, sizeof(info_log), 0, source);
            fprintf(stderr, "%s\n", info_log);
        }
    }
    
    return result;
}


static GLuint gl_utility_link_program(GL_Utility_Compiled_Shaders *compiled_shader_ids) {
    GLuint result = glCreateProgram();
    
    int count = (sizeof(GL_Utility_Compiled_Shaders) / sizeof(GLuint));
    GLuint *id_list = (GLuint *)compiled_shader_ids;
    
    for(int id_index = 0; id_index < count; ++id_index)
        glAttachShader(result, id_list[id_index]);
    
    glLinkProgram(result);
    
    GLint link_status;
    glGetProgramiv(result, GL_LINK_STATUS, &link_status);
    if(link_status == 0) {
        char info_log[INFO_LOG_LENGTH] = {};
        glGetProgramInfoLog(result, INFO_LOG_LENGTH, NULL, info_log);
        fprintf(stderr, "%s\n", info_log);
    }
    
    // TODO(lmk): Can this be done before linking?
    for(int id_index = 0; id_index < count; ++id_index)
        glDeleteShader(id_list[id_index]);
    
    *compiled_shader_ids = {};
    
    return result;
}


static GLint gl_utility_get_uniform_location(GLuint program, const GLchar *name) {
    GLint result = glGetUniformLocation(program, name);
    
    if(result == -1) {
        fprintf(stderr, "Uniform (%s) was not found in program %d\n", name, program);
        assert(0);
    }
    
    return result;
}

#endif //SHADER_H
