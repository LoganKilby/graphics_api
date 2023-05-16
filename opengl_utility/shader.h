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

static char *global_gl_shape_vert =
#include "shaders/shape.vert"
;
static char *global_gl_shape_frag =
#include "shaders/shape.frag"
;
static char *global_gl_texture_vert = 
#include "shaders/texture.vert"
;
static char *global_gl_texture_frag =
#include "shaders/texture.frag"
;
static char *global_gl_mvp_static_color_vert =
#include "shaders/transform_mvp_static_color.vert"
;

#define MAX_INFO_LOG_LENGTH 1024

static bool gl_check_compile_status(GLuint id) {
    
    
    return true;
}


static GLuint gl_compile_shader(char *source, GLint src_length, GLenum type) {
    GLuint result = glCreateShader(type);
    
    // NOTE(lmk): This was an annoying bug to find. When compiling null terminated source code, pass
    // null as the length parameter to glShaderSource, NOT THE ADDRESS OF THE VARIABLE STORING NULL!
    if(src_length == 0)
        glShaderSource(result, 1, &source, 0);
    else
        glShaderSource(result, 1, &source, &src_length);
    
    glCompileShader(result);
    
    GLint compile_status;
    glGetShaderiv(result, GL_COMPILE_STATUS, &compile_status);
    
    if(compile_status == GL_FALSE) {
        char info_log[MAX_INFO_LOG_LENGTH] = {};
        
        int suggested_length;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &suggested_length);
        
        GLsizei actual_length;
        glGetShaderInfoLog(result, sizeof(info_log), &actual_length, info_log);
        
        fprintf(stderr, "%s\n", info_log);
        assert(0);
        return false;
    }
    
    return result;
}


static GLuint gl_link_program(GL_Utility_Compiled_Shaders *compiled_shader_ids) {
    GLuint result = glCreateProgram();
    
    int count = (sizeof(GL_Utility_Compiled_Shaders) / sizeof(GLuint));
    GLuint *id_list = (GLuint *)compiled_shader_ids;
    
    for(int id_index = 0; id_index < count; ++id_index) {
        if(id_list[id_index])
            glAttachShader(result, id_list[id_index]);
    }
    
    glLinkProgram(result);
    
    GLint link_status;
    glGetProgramiv(result, GL_LINK_STATUS, &link_status);
    if(link_status == GL_FALSE) {
        char info_log[MAX_INFO_LOG_LENGTH] = {};
        
        int suggested_length = 0;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &suggested_length);
        
        int actual_length = 0;
        glGetProgramInfoLog(result, MAX_INFO_LOG_LENGTH, &actual_length, info_log);
        
        // Linking an invalid compiled shader id?
        //if((suggested_length == 0) && (actual_length > 0))
        //fprintf(stderr, "wtf");
        
        fprintf(stderr, "%s\n", info_log);
        assert(0);
    }
    
    // TODO(lmk): Can this be done before linking?
    for(int id_index = 0; id_index < count; ++id_index)
        glDeleteShader(id_list[id_index]);
    
    return result;
}


static GLint gl_get_uniform_location(GLuint program, const GLchar *name) {
    GLint result = glGetUniformLocation(program, name);
    
    if(result == -1) {
        fprintf(stderr, "Uniform (%s) was not found in program %d\n", name, program);
        gl_assert(0);
    }
    
    return result;
}

#endif //SHADER_H
