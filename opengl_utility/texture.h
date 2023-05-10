/* date = May 10th 2023 10:40 am */

#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

struct GL_Texture2D {
    GLuint id;
    int widht;
    int height;
};


struct GL_Image {
    unsigned char *data;
    int width;
    int height;
    int channels;
};

GL_Texture2D gl_texture_2d(GL_Image *image) {
    GL_Texture2D result = {};
    
    if(image->data) {
        glGenTextures(1, &result.id);
        glBindTexture(GL_TEXTURE_2D, result.id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    return result;
}


GL_Image gl_load_image(char *file, int required_comp = 0) {
    GL_Image result = {};
    result.data = stbi_load(file, &result.width, &result.height, &result.channels, required_comp);
    return result;
}


void gl_free_image(GL_Image *image) {
    free(image->data);
    *image = {};
}


GL_Texture2D gl_texture_2d(char *filename) {
    GL_Image image = gl_load_image(filename);
    assert(image.data);
    GL_Texture2D result = gl_texture_2d(&image);
    gl_free_image(&image);
    return result;
}


#endif //TEXTURE_H
