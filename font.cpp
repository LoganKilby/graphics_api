#include "stb_rect_pack.h"

struct Font_Atlas {
    int pixel_size;
};

struct Glyph_Dimensions {
    int width;
    int height;
};


Glyph_Rect *gather_glyph_rects(FT_Face face, int *count) {
    *count = 0;
    
    int num_glyphs = face.num_glyphs;
    int glyphs_added = 0;
    Glyph_Rect *result = (Glyph_Dimensions *)malloc(sizeof(Glyph_Dimensions) * num_glyphs);
    memset(result, 0, sizeof(Glyph_Dimensions) * num_glyphs);
    
    for(int glyph_index = 0; glyph_index < num_glyphs; ++glyph_index) {
        if(FT_Load_Glyph(face, glyph_index, FT_LOAD_BITMAP_METRICS_ONLY) == 0) {
            result[glyph_index].width = face.glyph_slot;
            result[glyph_index].height;
        } else {
            assert(0); // a glyph didn't have a bitmap.. is that normal?
        }
    }
    
    
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
    
    int num_faces = face->num_faces;
    int num_glyphs = face->num_glyphs;
    int face_index = face->face_index;
    
    assert(pixel_size >= 1);
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    
    Font_Atlas result = {};
    result.pixel_size = pixel_size;
}
