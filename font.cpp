struct Font_Atlas {
    int pixel_size;
};


void load_font_atlas(char *font_file_path, int pixel_size) {
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
    
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    
    Font_Atlas result = {};
    result.pixel_size = pixel_size;
}
