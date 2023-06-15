#include "stddef.h"

#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_ASSERT assert
#include "stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT assert
#include "stb_image_write.h"

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


struct Font_Texture_Atlas_Location {
    Rect bitmap; // texture atlas coordinates
    v2i bearing;
    int advance;
};


struct Font_Texture_Atlas {
    Font_Texture_Atlas_Location location[128];
    GL_Texture2D texture;
};


struct Font_Vertex {
    v2 pos;
    v2 uv;
    v3 color;
    float t;
};


struct Font {
    Font_Texture_Atlas atlas;
    Kerning_Table kerning;
    int pixel_size;
    
    Font_Vertex *get_vertices(char *str, int screen_x, int screen_y, float scale, v3 color, int *count);
};


struct GLS_Font2D {
    GLuint program;
    GLint u_sampler2D;
    GLint u_projection;
    GLint u_color;
    GLint u_t;
    
    void create(char *vert_shader = "shaders/font_shader.vert", char *frag_shader = "shaders/font_shader.frag") {
        GL_Compiled_Shaders s = {};
        s.vert = gl_compile_shader(vert_shader, GL_VERTEX_SHADER);
        s.frag = gl_compile_shader(frag_shader, GL_FRAGMENT_SHADER);
        program = gl_link_program(&s);
        u_sampler2D = gl_get_uniform_location(program, "u_texture");
        u_projection = gl_get_uniform_location(program, "u_projection");
        u_color = gl_get_uniform_location(program, "u_color");
        u_t = gl_get_uniform_location(program, "u_t");
    }
};


struct Font_Draw_Command {
    Font *font;
    v3 color;
    
    // NOTE(lmk): Expressed as seconds
    float delay;
    float duration;
    float elapsed;
    float t;
    
    int vertex_count;
    Font_Vertex *vertices;
    
    b32 active;
};


struct Font_Renderer {
    GLenum          usage;
    GL_Array_Buffer buffer;
    GLS_Font2D      shader;
    
    // TODO(lmk): Think of a good way to do command lists
    int command_list_count;
    Font_Draw_Command command_list[128];
    
    void create();
    void text(Font *font, char *str, int screen_x, int screen_y, f32 scale, v3 color); // renders text immediately
    void fade(Font *font, char *str, int screen_x, int screen_y, f32 scale, v3 color, f32 delay, f32 duration);
    void push(Font_Draw_Command *command);
    void render(mat4 *proj_2d);
};


void Font_Renderer::create() {
    shader.create();
    
    // NOTE(lmk): The vertex buffer is re-allocated per draw call and each quad in a string of text
    // gets drawn in the same call. This was faster than calling glBufferSubData and emitting a
    // draw call for each quad in the string.
    
    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Font_Vertex), (void *)offsetof(Font_Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Font_Vertex), (void *)offsetof(Font_Vertex, uv));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Font_Renderer::render(mat4 *projection_2d) {
    if(command_list_count == 0) return;
    
    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.u_projection, 1, GL_FALSE, (f32*)projection_2d);
    glActiveTexture(GL_TEXTURE0);
    
    for(Font_Draw_Command *command = &command_list[0]; 
        command < &command_list[0] + countof(command_list); 
        ++command) {
        
        if(command->active) {
            glUniform3fv(shader.u_color, 1, (f32 *)&command->color);
            glUniform1f(shader.u_t, command->t);
            glBindTexture(GL_TEXTURE_2D, command->font->atlas.texture.id);
            glBindVertexArray(buffer.vao);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Font_Vertex) * command->vertex_count, (f32 *)command->vertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, command->vertex_count);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // TODO(lmk): This feels out of place. Maybe the animation update should happen in a separate step?
            if(command->elapsed >= command->duration) {
                free(command->vertices);
                memset(command, 0, sizeof(Font_Draw_Command));
                command_list_count--;
            } else {
                if(command->delay > 0) {
                    command->delay -= Platform.delta_time;
                } else {
                    command->elapsed += Platform.delta_time;
                    command->t = command->elapsed / command->duration;
                }
            }
        }
    }
}


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


void minimum_bounding_box_solver(stbrp_rect *rects, int rect_count, int *width_out, int *height_out) {
    // (https://arxiv.org/ftp/arxiv/papers/1402/1402.0557.pdf : 3.3.1 Anytime Algorithm)
    // 1. find a greedy solution on a bounding box whose height is equal to the tallest rectangle
    // 2. Call containment solver
    // 3. IF the previous attempt succeeded, or if its area is greater than the area of the best solution seen so far, decrease the width
    // by one unit and repeat.
    //    ELSE if the previous attemp failed, increase the height of the bounding box by one unit and repeat.
    // 4. Terminate when the width of the bounding box is less than the width of the widest rectangle.
    
    // NOTE(lmk): This is reallyyy slow, but works for now...
    
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


void load_font(Font *font, char *font_file_path, int pixel_size) {
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
    font->pixel_size = pixel_size;
    
    u32 rect_count;
    stbrp_rect *rects = gather_glyph_rects(face, &rect_count);
    
    // TODO(lmk): There's a lot we could cache to speed this up in exchange for flexibility.
    // Caching the texture, the min bbox dimensions, etc.
    int bbox_width, bbox_height;
    minimum_bounding_box_solver(rects, rect_count, &bbox_width, &bbox_height);
    
    font->atlas.texture.width = bbox_width;
    font->atlas.texture.height = bbox_height;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &font->atlas.texture.id);
    glBindTexture(GL_TEXTURE_2D, font->atlas.texture.id);
    
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
            font->atlas.location[ascii_code].bitmap = {
                rect->x,
                rect->y,
                (int)face->glyph->bitmap.width,
                (int)face->glyph->bitmap.rows
            };
            
            font->atlas.location[ascii_code].bearing = v2i(face->glyph->bitmap_left, face->glyph->bitmap_top);
            font->atlas.location[ascii_code].advance = (u32)face->glyph->advance.x >> 6;
            
            get_lr_kerning_distance(face, &font->kerning, ascii_code);
            
            // TODO(lmk): Would it be faster to create our own image and and pass to OpenGL in a single call?
            glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y, rect->w, rect->h, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


void Font_Renderer::push(Font_Draw_Command *command) {
    if(command_list_count < countof(command_list)) {
        command_list[command_list_count++] = *command;
    } else {
        assert(0);
    }
}


void Font_Renderer::text(Font *font, char *str, int screen_x, int screen_y, float scale, v3 color) {
    int count = 0;
    Font_Vertex *vertices = font->get_vertices(str, screen_x, screen_y, scale, color, &count);
    
    Font_Draw_Command command = {};
    command.font = font;
    command.vertices = vertices;
    command.vertex_count = count;
    command.color = color;
    command.active = 1;
    
    push(&command);
}


void Font_Renderer::fade(Font *font, char *str, int screen_x, int screen_y, f32 scale, v3 color, f32 delay, f32 duration) {
    int count = 0;
    Font_Vertex *vertices = font->get_vertices(str, screen_x, screen_y, scale, color, &count);
    
    Font_Draw_Command command = {};
    command.font = font;
    command.vertices = vertices;
    command.vertex_count = count;
    command.color = color;
    command.delay = delay;
    command.duration = duration;
    command.active = 1;
    
    push(&command);
}


Font_Vertex *Font::get_vertices(char *str, int screen_x, int screen_y, float scale, v3 color, int *vertex_count) {
    size_t length = strlen(str);
    if(length == 0) {
        *vertex_count = 0;
        return 0;
    }
    
    // TODO(lmk): Think of a way to buffer up vertices every frame that doesn't involve heap allocations like this
    Font_Vertex *result = (Font_Vertex *)malloc(sizeof(Font_Vertex) * length * 6);
    memset(result, 0, sizeof(Font_Vertex) * length);
    *vertex_count = (int)length * 6;
    
    char previous_char = -1;
    char current_char;
    Font_Vertex *vertex;
    f32 atlas_width = (f32)atlas.texture.width;
    f32 atlas_height = (f32)atlas.texture.height;
    for (size_t str_index = 0; str_index < length; ++str_index) {
        current_char = str[str_index];
        vertex = &result[str_index * 6];
        
        Font_Texture_Atlas_Location *location = &atlas.location[current_char];
        
        f32 x = screen_x + (location->bearing.x * scale);
        f32 y = screen_y - (location->bitmap.height - location->bearing.y) * scale;
        f32 w = location->bitmap.width * scale;
        f32 h = location->bitmap.height * scale;
        
        f32 left = location->bitmap.x / atlas_width;
        f32 right = (location->bitmap.x + location->bitmap.width) / atlas_width;
        f32 bottom =  (location->bitmap.y / atlas_height);
        f32 top = (location->bitmap.y + location->bitmap.height) / atlas_height;
        
        vertex[0].pos = {x, y+h};
        vertex[0].uv = { left, bottom};
        vertex[0].color = color;
        vertex[0].t = 0;
        
        vertex[1].pos = {x, y};
        vertex[1].uv = {left, top};
        vertex[1].color = color;
        vertex[1].t = 0;
        
        vertex[2].pos = {x+w, y};
        vertex[2].uv = {right, top};
        vertex[2].t = 0;
        vertex[2].color = color;
        
        // TODO(lmk): Could use an element buffer
        vertex[3] = vertex[0];
        vertex[4] = vertex[2];
        
        vertex[5].pos = {x+w, y+h};
        vertex[5].uv = {right, bottom};
        vertex[5].color = color;
        vertex[5].t = 0;
        
        // advance cursors for next glyph
        screen_x += (int)(location->advance * scale);
        
        if(previous_char != -1) 
            screen_x += kerning.left[previous_char].right[current_char].x; 
    }
    
    return result;
}
