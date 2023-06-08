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


Glyph_Dimensions *gather_glyph_dimensions(FT_Face face, u32 *count) {
    STBRP_ASSERT(0);
    
    *count = 0;
    int num_glyphs = face->num_glyphs;
    int glyphs_added = 0;
    Glyph_Dimensions *result = (Glyph_Dimensions *)malloc(sizeof(Glyph_Dimensions) * num_glyphs);
    memset(result, 0, sizeof(Glyph_Dimensions) * num_glyphs);
    
    printf("%d\n", num_glyphs);
    
    for(int glyph_index = 0; glyph_index < num_glyphs; ++glyph_index) {
        if(FT_Load_Glyph(face, glyph_index, FT_LOAD_BITMAP_METRICS_ONLY) == 0) {
            result[glyph_index].width;
            result[glyph_index].height;
        } else {
            assert(0); // a glyph didn't have a bitmap.. is that normal?
        }
    }
    
    return result;
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
    
    //u32 glyph_count;
    //Glyph_Dimensions *glyph_dims = gather_glyph_dimensions(face, &glyph_count);
    
    pixel_size = 64;
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    
    int char_index = FT_Get_Char_Index(face, 'M');
    assert(FT_Load_Glyph(face, char_index, FT_LOAD_DEFAULT) == 0);
    assert(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) == 0);
    FT_Bitmap *bitmap = &face->glyph->bitmap;
    assert(bitmap->width && bitmap->rows);
    
    GL_Image image = {};
    u32 image_size = bitmap->rows * bitmap->width * bitmap->pitch;
    image.pixels = (unsigned char *)malloc(image_size);
    
    struct RGBA32 {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    
    
    for(u32 i = 0; i < bitmap->width * bitmap->rows; ++i) {
        RGBA32 *pixel = (RGBA32 *)&image.pixels[i * 4];
        pixel->r = bitmap->buffer[i];
        pixel->g = bitmap->buffer[i];
        pixel->b = bitmap->buffer[i];
        pixel->a = 255;
    }
    
    image.width = bitmap->width;
    image.height = bitmap->rows;
    memcpy(image.pixels, bitmap->buffer, image_size);
    
    int stride_in_bytes = bitmap->pitch / bitmap->width;
    assert(stbi_write_png("m.png", image.width, image.height, 8, image.pixels, stride_in_bytes));
    
    Font_Atlas result = {};
    result.pixel_size = pixel_size;
}
