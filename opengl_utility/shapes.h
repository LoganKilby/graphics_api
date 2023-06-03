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

struct GL_Rect {
    v2 tr; // top-right
    v2 br; // bottom-right
    v2 bl; // bottom-left
    v2 tl; // top-left
};

static GL_Rect gl_rect_texture_coords = {
    {1, 1},
    {1, 0},
    {0, 0},
    {0, 1},
};


GLuint rect_indices[] = {
    0, 1, 3,
    1, 2, 3
};

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
    glUniform4f(color_location, color.r, color.g, color.g, color.a);
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
    
    
    mat4 model = translate(mat4(1.0f), position) * rotate(basis);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->projection_3d);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (f32 *)&gl_utility_context_ptr->view_3d);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (f32 *)&model);
    glUniform4f(color_location, color.r, color.g, color.g, color.a);
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

#endif //SHAPES_H
