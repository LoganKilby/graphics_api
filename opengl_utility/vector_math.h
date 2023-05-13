/* date = May 3rd 2023 9:33 pm */

#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#ifdef GLM_VERSION
typedef glm::vec2 v2;
typedef glm::vec3 v3;
typedef glm::vec4 v4;
typedef glm::mat4 mat4;
#define M_PI glm::pi<float>()
#define radians(d) glm::radians<float>(d)
#else
#erorr GLM should be included
struct v2 {
    float x;
    float y;
};
struct v3 {
    union {
        float x, y, z;
    };
    
    union {
        float r, g, b;
    };
};
union v4 {
    struct {
        float x, y, z, w;
    };
    
    struct {
        float r, g, b, a;
    };
    
};
#endif

// NOTE(lmk): glm ortho -- (left, right, bottom, top, near, far)

v2 screen_to_ndc(v2 screen_pos, int screen_width, int screen_height) {
    v2 result;
    result.x = ((screen_pos.x / screen_width)) * 2 - 1;
    result.y = ((screen_pos.y / screen_height)) * 2 - 1;
    return result;
}

#endif //VECTOR_MATH_H
