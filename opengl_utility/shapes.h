/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H


#ifndef GLM_VERSION
#include "vector_math.h"
#else
#define v3 glm::vec3
#define v4 glm::vec4
#endif

struct Quad {
    v3 a;
    v3 b;
    v3 c;
    v3 d;
};

void gl_utility_draw_rect(v3 a, v3 b, v3 c, v3 d, v4 color) {
    v3 quad_vertex_buffer[6] = {
        a, b, c,
        b, c, d
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, global_gl_utility_context.shapes.quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer), quad_vertex_buffer, GL_DYNAMIC_DRAW);
    glUseProgram(global_gl_utility_context.static_color_program);
    
}

#endif //SHAPES_H
