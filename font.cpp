bool intialize_freetype(FT_Library *ft) {
    if(FT_Init_FreeType(ft)) {
        fprintf(stderr, "FreeType::failed to initialize.\n");
        return false;
    }
    
    return true;
}

bool load_font(FT_Library freetype, FT_Face *face, char *font_name) {
    int face_index = 0;
    if(FT_New_Face(freetype, font_name, face_index, face)) {
        fprintf(stderr, "FreeType::failed to load font %s\n", font_name);
        return false;
    }
    
    return true;
}