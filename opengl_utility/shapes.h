/* date = May 3rd 2023 9:23 pm */

#ifndef SHAPES_H
#define SHAPES_H

#include "vector_math.h"

struct Quad {
    v3 a;
    v3 b;
    v3 c;
    v3 d;
};

void gl_quad(v3 a, v3 b, v3 c, v3 d, v3 color) {
    v3 quad_vertex_buffer[6] = {
        a, b, c,
        b, c, d
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, global_gl_utility_context.shapes.quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer), quad_vertex_buffer, GL_DYNAMIC_DRAW);
}

#endif //SHAPES_H
