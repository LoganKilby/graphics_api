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

#include "shader.h"

struct Quad {
    v3 a;
    v3 b;
    v3 c;
    v3 d;
};

void gl_draw_rect(v3 a, v3 b, v3 c, v3 d, v4 color) {
    v3 quad_vertex_buffer[6] = {
        a, b, c,
        b, c, d
    };
    
    glBindVertexArray(gl_utility_context_ptr->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->shape_vbo.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer), quad_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLES, 0, countof(quad_vertex_buffer));
}


void gl_draw_triangle(v3 a, v3 b, v3 c, v4 color) {
    v3 triangle_vertex_buffer[3] = {
        a, b, c
    };
    
    glBindVertexArray(gl_utility_context_ptr->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->shape_vbo.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex_buffer), triangle_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLES, 0, countof(triangle_vertex_buffer));
}


void gl_draw_polygon(v3 *a, unsigned int count, v4 color) {
    glBindVertexArray(gl_utility_context_ptr->shape_vao.v3f);
    glBindBuffer(GL_ARRAY_BUFFER, gl_utility_context_ptr->shape_vbo.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * count, a, GL_DYNAMIC_DRAW);
    glUseProgram(gl_utility_context_ptr->static_color_program);
    glUniform4f(gl_utility_context_ptr->static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
}


void gl_draw_circle(float radius, int vertex_count) {
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

#endif //SHAPES_H
