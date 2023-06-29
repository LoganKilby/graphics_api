/* date = June 20th 2023 0:02 pm */

#ifndef FONT_H
#define FONT_H


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
    // NOTE(lmk): Using max bitmap dimension for spacing the ' ' character and for new lines
    int max_bitmap_width;
    int max_bitmap_height;
    
    Font_Texture_Atlas_Location location[128];
    GL_Texture2D texture;
};


struct Font_Vertex {
    v2 position;
    v2 uv;
    v3 color;
};


struct Font {
    Font_Texture_Atlas atlas;
    Kerning_Table kerning;
    int pixel_size;
    
    void get_vertices(char *str, int str_len, v2i screen_pos, float scale, v3 color, Font_Vertex *vertices);
    int text_width(char *str, float scale);
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
    f32 t;
    
    int vertex_count;
    Font_Vertex *vertices;
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
    void push(Font_Draw_Command *command);
    void render(mat4 *proj_2d);
};


struct Font_Fade {
    f32 delay_elapsed;
    f32 fade_elapsed;
};

#define MAX_MSG_NOTIFICATIONS 16
struct Msg_Data {
    Font_Fade fade;
    Font_Vertex *vertices;
    int vertex_count;
};

// stacks multiples instances of text, drawn separately with a fade out effect
struct Msg_Notifier {
    int msg_count;
    Msg_Data data[MAX_MSG_NOTIFICATIONS];
    
    f32 delay; // 1
    f32 duration; // 1
    
    v2i origin;
    f32 scale;
    v3 color;
    
    Font *font;
    
    Font_Renderer *renderer;
    
    void create(Font_Renderer *renderer, Font *font, v2i origin, v3 color, f32 scale);
    void push_message(char *msg);
    void update();
};


#endif //FONT_H
