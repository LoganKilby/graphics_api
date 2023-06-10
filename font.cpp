#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_ASSERT assert
#include "stb_rect_pack.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT assert
#include "stb_image_write.h"


struct Font_Atlas {
    int pixel_size;
};


struct Glyph_Dimensions {
    int width;
    int height;
};


Glyph_Dimensions *gather_alphanumeric_glyph_dimensions(FT_Face face, u32 *count) {
    Glyph_Dimensions *result = (Glyph_Dimensions *)malloc(sizeof(Glyph_Dimensions) * 128);
    memset(result, 0, sizeof(Glyph_Dimensions) * 128);
    
    // TODO(lmk): figure out spacing for new line and space character
    int bbox_width = (face->bbox.xMax - face->bbox.xMin) >> 6;
    int bbox_height = (face->bbox.yMax - face->bbox.yMin) >> 6;
    
    int glyphs_loaded = 0;
    for(int ascii_char_code = 0; ascii_char_code < 128; ++ascii_char_code) {
        if(FT_Load_Char(face, ascii_char_code, FT_LOAD_BITMAP_METRICS_ONLY) == 0) {
            //assert(face->glyph->metrics.width > 0);
            //assert(face->glyph->metrics.height > 0);
            //result[result_index].width = face->glyph->metrics.width >> 6;
            //result[result_index].height = face->glyph->metrics.height >> 6;
            if(face->glyph->bitmap.width && face->glyph->bitmap.rows) {
                result[glyphs_loaded].width = face->glyph->bitmap.width;
                result[glyphs_loaded].height = face->glyph->bitmap.rows;
                ++glyphs_loaded;
            } else {
                fprintf(stderr, "FreeType::FT_Load_Char(): glyph does not have bitmap: %c\n", ascii_char_code);
            }
        } else {
            fprintf(stderr, "FreeType::FT_Load_Char(): failed to load char %c\n", ascii_char_code);
        }
    }
    
    *count = glyphs_loaded;
    
    return result;
}

struct Font {
    u32 tex_id[128];
    v2 size[128];
    v2 bearing[128];
    u32 advance[128];
    
    u32 vbo;
    u32 vao;
};


void load_ascii_textures(Font *font, char *font_file_path, int pixel_size) {
    memset(font, 0, sizeof(Font));
    
    FT_Library library;
    if(FT_Init_FreeType(&library)) {
        fail("FreeType::FT_Init_FreeType(): failed to initialize library");
        return;
    }
    
    FT_Face face;
    if(FT_New_Face(library, font_file_path, 0, &face)) {
        fail("FreeType::FT_New_Face(): failed to load font");
        return;
    }
    
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for(unsigned char c = 0; c < 128; ++c) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "FreeType::FT_Load_Char(): failed to load char %d\n", c);
            continue;
        }
        
        glGenTextures(1, &font->tex_id[c]);
        glBindTexture(GL_TEXTURE_2D, font->tex_id[c]);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        font->size[c] = v2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        font->bearing[c] = v2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        font->advance[c] = face->glyph->advance.x;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    
    glGenVertexArrays(1, &font->vao);
    glGenBuffers(1, &font->vbo);
    glBindVertexArray(font->vao);
    glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void create_font_atlas(char *font_file_path, int pixel_size) {
    // TODO(lmk): may want to limit the pixel size
    int max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    
    FT_Library library;
    if(FT_Init_FreeType(&library)) {
        fail("FreeType::FT_Init_FreeType(): failed to initialize library");
        return;
    }
    
    FT_Face face;
    if(FT_New_Face(library, font_file_path, 0, &face)) {
        fail("FreeType::FT_New_Face(): failed to load font");
    }
    
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    
    //u32 dim_count;
    //Glyph_Dimensions *dims = gather_alphanumeric_glyph_dimensions(face, &dim_count);
    
    int glyph_index = FT_Get_Char_Index(face, 32);
    assert(FT_Load_Char(face, 32, FT_LOAD_RENDER) == 0);
    //assert(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) == 0);
    FT_Bitmap *bitmap = &face->glyph->bitmap;
    assert(stbi_write_png("m.png", bitmap->width, bitmap->rows, 1, bitmap->buffer, bitmap->pitch));
    
    Font_Atlas result = {};
    result.pixel_size = pixel_size;
}


void render_font(Font *font, char *str, float x, float y, float scale, mat4 *projection, u32 program) {
    glUseProgram(program);
    glUniform3f(gl_get_uniform_location(program, "textColor"), 1, 1, 1);
    glUniformMatrix4fv(gl_get_uniform_location(program, "projection"), 1, GL_FALSE, (f32 *)projection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->vao);
    
    // iterate through all characters
    size_t length = strlen(str);
    for (size_t str_index = 0; str_index < length; ++str_index)
    {
        int c = str[str_index];
        float xpos = x + font->bearing[str[c]].x * scale;
        float ypos = y - (font->size[c].y - font->bearing[c].y) * scale;
        
        float w = font->size[c].x * scale;
        float h = font->size[c].y * scale;
        
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, font->tex_id[c]);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (font->advance[c] >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}