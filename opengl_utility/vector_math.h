/* date = May 3rd 2023 9:33 pm */

#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#ifdef GLM_VERSION
using namespace glm;
typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;
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

#define EPSILON 0.0006

inline bool zero_vector(v3 v) {
    bool result = 
        (abs(v.x) < EPSILON) &&
        (abs(v.y) < EPSILON) &&
        (abs(v.z) < EPSILON);
    
    return result;
}

// NOTE(lmk): glm ortho -- (left, right, bottom, top, near, far)

v2 screen_to_ndc(v2 screen_pos, int screen_width, int screen_height) {
    v2 result;
    result.x = ((screen_pos.x / screen_width)) * 2 - 1;
    result.y = ((screen_pos.y / screen_height)) * 2 - 1;
    return result;
}

mat4 look_at(v3 camera_pos, v3 up, v3 target) {
    mat4 result(1.0f);
    
    //result[0] = 
    
    return result;
}

#endif //VECTOR_MATH_H
