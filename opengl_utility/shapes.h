/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H

#ifndef GLM_VERSION
#include "vector_math.h"
#else
typedef glm::vec2 v2;
typedef glm::vec3 v3;
typedef glm::vec4 v4;
#define M_PI glm::pi<float>()
#endif


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
    v3 tr; // top-right
    v3 br; // bottom-right
    v3 bl; // bottom-left
    v3 tl; // top-left
};


inline void gl_rect(GL_Rect r, v4 color) {
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    
    glBindVertexArray(gl_utility_context_ptr->rect_3f.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_Rect), (float *)&r, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_utility_context_ptr->rect_3f.ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // NOTE(lmk): The indices define 6 vertices from 4
}


inline void gl_rect(v3 a, v3 b, v3 c, v3 d, v4 color) {
    gl_rect({a, b, c, d}, color);
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
