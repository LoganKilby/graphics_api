#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_ASSERT assert
#include "stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT assert
#include "stb_image_write.h"


struct Font_Atlas {
    int pixel_size;
};


// NOTE(lmk): Only loading ascii 0-128. Could trivially support more, I just don't have any use for them.
stbrp_rect *gather_glyph_rects(FT_Face face, u32 *count) {
    stbrp_rect *result = (stbrp_rect *)malloc(sizeof(stbrp_rect) * 128);
    memset(result, 0, sizeof(stbrp_rect) * 128);
    
    int glyphs_loaded = 0;
    for(int ascii_char_code = 0; ascii_char_code < 128; ++ascii_char_code) {
        if(FT_Load_Char(face, ascii_char_code, FT_LOAD_BITMAP_METRICS_ONLY) == 0) {
            if(face->glyph->bitmap.width && face->glyph->bitmap.rows) {
                result[glyphs_loaded].w = face->glyph->bitmap.width;
                result[glyphs_loaded].h = face->glyph->bitmap.rows;
                result[glyphs_loaded].id = ascii_char_code;
                ++glyphs_loaded;
            } else {
                fprintf(stderr, "FreeType::FT_Load_Char(): glyph does not have bitmap: %c\n", ascii_char_code);
            }
        } else {
            fprintf(stderr, "FreeType::FT_Load_Char(): failed to load char %c\n", ascii_char_code);
        }
    }
    
    realloc(result, sizeof(stbrp_rect) * glyphs_loaded);
    
    *count = glyphs_loaded;
    
    return result;
}


struct Kern_Left {
    v2i right[128];
};

struct Kern_Right {
    v2i left[128];
};

struct Kerning_Table {
    Kern_Left left[128];
};

struct Font {
    u32 atlas_tex_id;
    u32 tex_id[128];
    v2i size[128];
    v2i bearing[128];
    u32 advance[128];
    Kerning_Table kerning;
    
    u32 vbo;
    u32 vao;
};


void get_lr_kerning_distance(FT_Face face, Kerning_Table *table, int c1) {
    if(FT_HAS_KERNING(face)) {
        FT_Vector delta;
        int g1, g2;
        
        g1 = FT_Get_Char_Index(face, c1);
        
        for(int c2 = 0; c2 < 128; ++c2) {
            g2 = FT_Get_Char_Index(face, c2);
            
            FT_Get_Kerning(face, g1, g2, FT_KERNING_DEFAULT, &delta);
            table->left[c1].right[c2] = { delta.x >> 6, delta.y >> 6 };
            
            FT_Get_Kerning(face, g2, g1, FT_KERNING_DEFAULT, &delta);
            table->left[c2].right[c1] = { delta.x >> 6, delta.y >> 6 };
        }
    }
}


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
        
        font->size[c] = v2i(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        font->bearing[c] = v2i(face->glyph->bitmap_left, face->glyph->bitmap_top);
        
        assert(face->glyph->advance.x >= 0);
        font->advance[c] = (u32)face->glyph->advance.x >> 6;
        
        get_lr_kerning_distance(face, &font->kerning, c);
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


void minimum_bounding_box_solver(stbrp_rect *rects, int rect_count, int *width_out, int *height_out) {
    // (https://arxiv.org/ftp/arxiv/papers/1402/1402.0557.pdf : 3.3.1 Anytime Algorithm)
    // 1. find a greedy solution on a bounding box whose height is equal to the tallest rectangle
    // 2. Call containment solver
    // 3. IF the previous attempt succeeded, or if its area is greater than the area of the best solution seen so far, decrease the width
    // by one unit and repeat.
    //    ELSE if the previous attemp failed, increase the height of the bounding box by one unit and repeat.
    // 4. Terminate when the width of the bounding box is less than the width of the widest rectangle.
    
    // NOTE(lmk): This is reallyyy slow
    
    TIMED_BLOCK;
    
    int tallest_height = 0;
    int widest_width = 0;
    int width_sum = 0;
    int height_sum = 0;
    for(stbrp_rect *rect = rects; rect < rects + rect_count; ++rect) {
        if(rect->h > tallest_height) tallest_height = rect->h;
        if(rect->w > widest_width) widest_width = rect->w;
        width_sum += rect->w;
        height_sum += rect->h;
    }
    
    int bbox_width = width_sum;
    int bbox_height = tallest_height;
    int bbox_area = bbox_width * bbox_height;
    
    int node_count = bbox_width;
    stbrp_node *nodes = (stbrp_node *)malloc(sizeof(stbrp_node) * node_count);
    memset(nodes, 0, sizeof(stbrp_node) * node_count);
    stbrp_context context = {};
    
    int current_bbox_width = bbox_width;
    int current_bbox_height = bbox_height;
    int current_bbox_area = current_bbox_width * current_bbox_height;
    
    int smallest_area = current_bbox_area;
    f32 smallest_diagonal = sqrtf((f32)squared(current_bbox_width) + (f32)squared(current_bbox_height));
    int width_result = 0;
    int height_result = 1;
    
    for(;;) {
        current_bbox_area = current_bbox_width * current_bbox_height;
        stbrp_init_target(&context, current_bbox_width, current_bbox_height, nodes, node_count);
        
        if(stbrp_pack_rects(&context, rects, rect_count)) {
            f32 diagonal = sqrtf((f32)squared(current_bbox_width) + (f32)squared(current_bbox_height));
            
            if(diagonal < smallest_diagonal) {
                smallest_diagonal = diagonal;
                smallest_area = current_bbox_area;
                width_result = current_bbox_width;
                height_result = current_bbox_height;
            }
            
            --current_bbox_width;
        } else if (current_bbox_area > smallest_area) {
            --current_bbox_width;
        } else {
            ++current_bbox_height;
        }
        
        if(current_bbox_width < widest_width) break;
    }
    
    stbrp_init_target(&context, width_result, height_result, nodes, node_count);
    stbrp_pack_rects(&context, rects, rect_count);
    *width_out = width_result;
    *height_out = height_result;
}


void create_font_atlas(Font *font, char *font_file_path, int pixel_size) {
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
    
    u32 rect_count;
    stbrp_rect *rects = gather_glyph_rects(face, &rect_count);
    
    // TODO(lmk): we could cache this result, or even the entire font atlas bitmap itself
    int bbox_width, bbox_height;
    minimum_bounding_box_solver(rects, rect_count, &bbox_width, &bbox_height);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &font->atlas_tex_id);
    glBindTexture(GL_TEXTURE_2D, font->atlas_tex_id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 bbox_width,
                 bbox_height,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    for(stbrp_rect *rect = rects; rect < rects + rect_count; ++rect) {
        int ascii_code = rect->id;
        if(FT_Load_Char(face, ascii_code, FT_LOAD_RENDER) == 0) {
            font->size[ascii_code] = v2i(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            font->bearing[ascii_code] = v2i(face->glyph->bitmap_left, face->glyph->bitmap_top);
            font->advance[ascii_code] = (u32)face->glyph->advance.x >> 6;
            get_lr_kerning_distance(face, &font->kerning, ascii_code);
            
            // TODO(lmk): Would it be faster to create our own image and and pass to OpenGL in a single call?
            glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y, rect->w, rect->h, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }
    }
    
    int glyph_index = FT_Get_Char_Index(face, 32);
    assert(FT_Load_Char(face, 32, FT_LOAD_RENDER) == 0);
    FT_Bitmap *bitmap = &face->glyph->bitmap;
    assert(stbi_write_png("m.png", bitmap->width, bitmap->rows, 1, bitmap->buffer, bitmap->pitch));
    
    Font_Atlas result = {};
    result.pixel_size = pixel_size;
    
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


void render_font(Font *font, char *str, float x, float y, float scale, mat4 *projection, u32 program) {
    glUseProgram(program);
    glUniform3f(gl_get_uniform_location(program, "textColor"), 1, 1, 1);
    glUniformMatrix4fv(gl_get_uniform_location(program, "projection"), 1, GL_FALSE, (f32 *)projection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->vao);
    
    // iterate through all characters
    size_t length = strlen(str);
    char previous_char = -1;
    char current_char;
    for (size_t str_index = 0; str_index < length; ++str_index)
    {
        current_char = str[str_index];
        float xpos = x + font->bearing[current_char].x * scale;
        float ypos = y - (font->size[current_char].y - font->bearing[current_char].y) * scale;
        
        float w = font->size[current_char].x * scale;
        float h = font->size[current_char].y * scale;
        
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
        glBindTexture(GL_TEXTURE_2D, font->tex_id[current_char]);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += font->advance[current_char] * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        
        if(previous_char != -1) 
            x += font->kerning.left[previous_char].right[current_char].x;
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
