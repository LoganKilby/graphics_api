/* date = May 10th 2023 10:40 am */

#ifndef TEXTURE_H
#define TEXTURE_H

#define STBI_ASSERT gl_assert
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

char *image_file_ext[] = {
    "jpg",
    "png",
    "bmp",
    "psd",
    "tga",
    "gif",
    "hdr",
    "pic",
    "pnm"
};

enum GL_Image_Extension {
    JPEG,
    PNG,
    BMP,
    PSD,
    TGA,
    GIF,
    HDR,
    PIC,
    PNM,
    EXTENSION_COUNT,
    INVALID_EXTENSION
};

struct GL_Texture2D {
    GLuint id;
    int width;
    int height;
};


struct GL_Image {
    GL_Image_Extension type;
    unsigned char *data;
    int width;
    int height;
    int channels;
};


GL_Texture2D gl_texture_2d(GL_Image *image) {
    GL_Texture2D result = {};
    
    if(image->data) {
        result.width = image->width;
        result.height = image->height;
        
        glGenTextures(1, &result.id);
        glBindTexture(GL_TEXTURE_2D, result.id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        switch(image->type) {
            case JPEG: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
            } break;
            
            case PNG: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
            } break;
            
            default: {
                gl_assert(0); // Invalid or unsupported extension
            }
        }
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    return result;
}


static GL_Image_Extension gl_get_image_file_extension(char *filename_nt) {
    int str_length = (int)strlen(filename_nt);
    
    int begin_ext_index = 0;
    for(int i = str_length - 1; i > 0; --i) {
        if(filename_nt[i] == '.') {
            begin_ext_index = i;
            break;
        }
    }
    
    if(begin_ext_index) {
        for(int ext_index = 0; ext_index < countof(image_file_ext); ++ext_index) {
            if(strcmp(&filename_nt[begin_ext_index + 1], image_file_ext[ext_index]) == 0) {
                return (GL_Image_Extension)ext_index;
            }
        }
    }
    
    return INVALID_EXTENSION;
}


GL_Image gl_load_image(char *file, int required_comp = 0) {
    GL_Image result = {};
    result.type = gl_get_image_file_extension(file);
    gl_assert(result.type != INVALID_EXTENSION);
    result.data = stbi_load(file, &result.width, &result.height, &result.channels, required_comp);
    return result;
}


void gl_free_image(GL_Image *image) {
    free(image->data);
    *image = {};
}


GL_Texture2D gl_texture_2d(char *filename) {
    GL_Image image = gl_load_image(filename);
    gl_assert(image.data);
    GL_Texture2D result = gl_texture_2d(&image);
    gl_free_image(&image);
    return result;
}


#endif //TEXTURE_H
