/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H

#define X_AXIS_COLOR4 v4(0, 0, 1, 1)
#define Y_AXIS_COLOR4 v4(0, 1, 0, 1)
#define Z_AXIS_COLOR4 v4(1, 0, 0, 1)

struct v5 {
    float x;
    float y;
    float z;
    float u;
    float v;
};

#include "shader.h"

struct GL_Quad {
    v3 a;
    v3 b;
    v3 c;
    v3 d;
};

typedef GL_Viewport GL_Rect;
typedef GL_Rect Rect;

static float cube_v3f_uv2f[] {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


static float cube_v3f[] {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};


inline f32 *get_vertices_cube(size_t *buffer_size, size_t *vertex_size) {
    f32 *result = (f32 *)cube_v3f_uv2f;
    *buffer_size = sizeof(cube_v3f_uv2f);
    *vertex_size = sizeof(cube_v3f_uv2f[0]);
    return result;
}


void gl_cube(v3 position, v4 color) {
    GL_Array_Buffer *buf = &gl_utility_context_ptr->array_buffer_v3f;
    glBindVertexArray(buf->vao);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v3f), cube_v3f, GL_STATIC_DRAW);
    
    int program = gl_utility_context_ptr->program_transform_static_color_v3f;
    glUseProgram(program);
    
    int view_location = gl_get_uniform_location(program, "u_view");
    int projection_location = gl_get_uniform_location(program, "u_projection");
    int model_location = gl_get_uniform_location(program, "u_model");
    int color_location = gl_get_uniform_location(program, "u_color");
    
    mat4 model = translate(mat4(1.0f), position);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->projection_3d);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->view_3d);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (f32 *)&model);
    glUniform4fv(color_location, 1, (float *)&color);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void gl_cube(v3 position, Basis *basis, v4 color) {
    GL_Array_Buffer *buf = &gl_utility_context_ptr->array_buffer_v3f;
    glBindVertexArray(buf->vao);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v3f), cube_v3f, GL_STATIC_DRAW);
    
    int program = gl_utility_context_ptr->program_transform_static_color_v3f;
    glUseProgram(program);
    
    int view_location = gl_get_uniform_location(program, "u_view");
    int projection_location = gl_get_uniform_location(program, "u_projection");
    int model_location = gl_get_uniform_location(program, "u_model");
    int color_location = gl_get_uniform_location(program, "u_color");
    
    mat4 model = translate(mat4(1.0f), position) * basis_to_mat4(basis);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->projection_3d);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->view_3d);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (f32 *)&model);
    glUniform4fv(color_location, 1, (float *)&color);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void gl_line(v3 a, v3 b, v4 color) {
    v3 buffer[] = {a, b};
    GL_Array_Buffer *buf = &gl_utility_context_ptr->array_buffer_v3f;
    glBindVertexArray(buf->vao);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    
    int program = gl_utility_context_ptr->program_transform_static_color_v3f;
    glUseProgram(program);
    
    int view_location = gl_get_uniform_location(program, "u_view");
    int projection_location = gl_get_uniform_location(program, "u_projection");
    int model_location = gl_get_uniform_location(program, "u_model");
    int color_location = gl_get_uniform_location(program, "u_color");
    
    mat4 model = mat4(1.0f);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->projection_3d);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->view_3d);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (f32 *)&model);
    glUniform4fv(color_location, 1, (f32 *)&color);
    glDrawArrays(GL_LINES, 0, 2);
}


void gl_basis(v3 origin, Basis *basis) {
    glDisable(GL_DEPTH_TEST);
    gl_line(origin, origin + basis->right, X_AXIS_COLOR4);
    gl_line(origin, origin + basis->up, Y_AXIS_COLOR4);
    gl_line(origin, origin + basis->front, Z_AXIS_COLOR4);
    glEnable(GL_DEPTH_TEST);
}


struct GL_Texture_Rect {
    GL_Array_Buffer buffer;
    GLS_Textured_Polygon shader;
    GLenum usage;
    
    void create(GLS_Textured_Polygon *_shader);
    void render(Rect rect, GLuint texture_id, mat4 *proj_2d);
    void render(Rect rect, Rect sample_rect, GL_Texture2D texture, mat4 *proj_2d);
};


void GL_Texture_Rect::create(GLS_Textured_Polygon *_shader) {
    shader = *_shader;
    usage = GL_DYNAMIC_DRAW;
    gl_array_buffer_2f2f(&buffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, 0, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GL_Texture_Rect::render(Rect rect, GLuint texture_id, mat4 *proj_2d) {
    f32 x = (f32)rect.x;
    f32 y = (f32)rect.y;
    f32 w = (f32)rect.width;
    f32 h = (f32)rect.height;
    
    float vertices[6][4] = {
        { x,     y + h, 0.0f, 0.0f },
        { x,     y,     0.0f, 1.0f },
        { x + w, y,     1.0f, 1.0f },
        
        { x,     y + h, 0.0f, 0.0f },
        { x + w, y,     1.0f, 1.0f },
        { x + w, y + h, 1.0f, 0.0f }
    };
    
    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.u_projection, 1, GL_FALSE, (f32*)proj_2d);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void GL_Texture_Rect::render(Rect rect, Rect sample_rect, Texture2D texture, mat4 *proj_2d) {
    f32 x = (f32)rect.x;
    f32 y = (f32)rect.y;
    f32 w = (f32)rect.width;
    f32 h = (f32)rect.height;
    
    f32 texture_width = (f32)texture.width;
    f32 texture_height = (f32)texture.height;
    
    f32 left = sample_rect.x / texture_width;
    f32 right = (sample_rect.x + sample_rect.width) / texture_width;
    f32 bottom =  (sample_rect.y / texture_height);
    f32 top = (sample_rect.y + sample_rect.height) / texture_height;
    
    float vertices[6][4] = {
        { x,     y + h,  left, bottom  },
        { x,     y,      left, top     },
        { x + w, y,      right, top    },
        
        { x,     y + h,  left, bottom  },
        { x + w, y,      right, top    },
        { x + w, y + h,  right, bottom }
    };
    
    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.u_projection, 1, GL_FALSE, (f32*)proj_2d);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif //SHAPES_H
