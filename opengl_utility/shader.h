/* date = May 1st 2023 7:17 pm */

#ifndef SHADER_H
#define SHADER_H

// NOTE(lmk): GLSL reference compiler uses the extensions .vert, .frag, .geom, .tesc, .tese, .comp
// It was claimed that this compiler 

#define INFO_LOG_LENGTH 1024

static GLint compile_shader(char *source, GLint source_length, GLenum type) {
    GLint result = -1;
    
    if(source) {
        result = glCreateShader(type);
        glShaderSource(result, 1, &source, &source_length);
        glCompileShader(result);
        
        GLint compile_status;
        glGetShaderiv(result, GL_COMPILE_STATUS, &compile_status);
        if(compile_status == GL_FALSE) {
            char info_log[INFO_LOG_LENGTH] = {};
            
            s32 info_log_length = 0;
            glGetShaderiv(compile_status, GL_INFO_LOG_LENGTH, &info_log_length);
            assert(info_log_length < sizeof(info_log));
            
            GLsizei bytes_read;
            glGetShaderInfoLog(result, sizeof(info_log), &bytes_read, source);
        }
    }
    
    return result;
}

#endif //SHADER_H
