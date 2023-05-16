/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H

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

static v5 cube_v3f_uv2f[] {
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


void gl_rect(GL_Rect r, v4 color) {
    v3 vertices[] = {
        v3(r.tr.x, r.tr.y, 0),
        v3(r.br.x, r.br.y, 0),
        v3(r.bl.x, r.bl.y, 0),
        v3(r.tl.x, r.tl.y, 0)
    };
    
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4fv(gl_utility_context_ptr->static_color_uniform_color, 1, (GLfloat *)&color);
    
    glBindVertexArray(gl_utility_context_ptr->rect_3f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float *)&vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f.ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOTE(lmk): The indices define 6 vertices from 4
}


inline void gl_rect(v3 a, v3 b, v3 c, v3 d, v4 color) {
    gl_rect({a, b, c, d}, color);
}

void gl_rect(v2 origin, float width, float height, v4 color) {
    float width_half = width/2;
    float height_half = height/2;
    
    // 0,0 at top left
    v4 tr = v4(origin.x + width_half, origin.y - height_half, 0, 1);
    v4 br = v4(origin.x + width_half, origin.y + height_half, 0, 1);
    v4 bl = v4(origin.x - width_half, origin.y + height_half, 0, 1);
    v4 tl = v4(origin.x - width_half, origin.y - height_half, 0, 1);
    
    GL_Rect r;
    r.tr = v2(gl_utility_context_ptr->ortho_2d * tr);
    r.br = v2(gl_utility_context_ptr->ortho_2d * br);
    r.bl = v2(gl_utility_context_ptr->ortho_2d * bl);
    r.tl = v2(gl_utility_context_ptr->ortho_2d * tl);
    
    gl_rect(r, color);
}

// NOTE(lmk): 0,0 at top left
void gl_rect(int left, int right, int top, int bottom, v4 color) {
    v4 tr = v4(right, top, 0, 1);
    v4 br = v4(right, bottom, 0, 1);
    v4 bl = v4(left, bottom, 0, 1);
    v4 tl = v4(left, top, 0, 1);
    
    GL_Rect r;
    r.tr = v2(gl_utility_context_ptr->ortho_2d * tr);
    r.br = v2(gl_utility_context_ptr->ortho_2d * br);
    r.bl = v2(gl_utility_context_ptr->ortho_2d * bl);
    r.tl = v2(gl_utility_context_ptr->ortho_2d * tl);
    
    gl_rect(r, color);
}

void gl_triangle(v3 a, v3 b, v3 c, v4 color) {
    v3 triangle_vertex_buffer[3] = {
        a, b, c
    };
    
    glBindVertexArray(gl_utility_context_ptr->rect_3f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex_buffer), triangle_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLES, 0, countof(triangle_vertex_buffer));
}

#if 0
void gl_polygon(v3 *a, unsigned int count, v4 color) {
    glBindVertexArray(gl_utility_context_ptr->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->shape_vbo.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * count, a, GL_DYNAMIC_DRAW);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
}


void gl_circle(float radius, int vertex_count) {
    glBindVertexArray(gl_utility_context_ptr->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->shape_vbo.v3f);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, 1, 1, 1, 1);
    
    v3 *vertices = (v3 *)SCRATCH_ALLOCATOR(sizeof(v3) * vertex_count);
    
    float t = 0;
    for(int i = 0; i < vertex_count; ++i) {
        vertices[i] = v3(1 + radius * cos(t), 1 + radius * sin(t), 0);
        t += 2 * M_PI;
    }
    
    glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
}
#endif


void gl_texture_rect(GL_Rect r, GL_Texture2D texture) {
    v5 vertex_buffer[4] = {
        {r.tr.x, r.tr.y, 0, 1, 1},
        {r.br.x, r.br.y, 0, 1, 0},
        {r.bl.x, r.bl.y, 0, 0, 0},
        {r.tl.x, r.tl.y, 0, 0, 1},
    };
    
    glUseProgram(gl_utility_context_ptr->texture_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    glBindVertexArray(gl_utility_context_ptr->rect_3f2f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f2f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f2f.ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOTE(lmk): The indices define 6 vertices from 4
}

#endif //SHAPES_H