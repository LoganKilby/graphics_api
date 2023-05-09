/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H


#ifndef GLM_VERSION
#include "vector_math.h"
#else
typedef glm::vec3 v3;
typedef glm::vec4 v4;
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
    
    glBindVertexArray(global_gl_utility_context.shape_vao.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer), quad_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(global_gl_utility_context.static_color_program);
    glDrawArrays(GL_TRIANGLES, 0, countof(quad_vertex_buffer));
    glBindVertexArray(0);
}


void gl_draw_triangle(v3 a, v3 b, v3 c, v4 color) {
    v3 triangle_vertex_buffer[3] = {
        a, b, c
    };
    
    glBindVertexArray(global_gl_utility_context.shape_vao.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex_buffer), triangle_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(global_gl_utility_context.static_color_program);
    glUniform4f(global_gl_utility_context.static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLES, 0, countof(triangle_vertex_buffer));
    glBindVertexArray(0);
}


void gl_draw_polygon(v3 *a, unsigned int count, v4 color) {
    glBindVertexArray(global_gl_utility_context.shape_vao.v3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * count, a, GL_DYNAMIC_DRAW);
    glUseProgram(global_gl_utility_context.static_color_program);
    glUniform4f(global_gl_utility_context.static_color_uniform_color, color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
    glBindVertexArray(0);
}

#endif //SHAPES_H
