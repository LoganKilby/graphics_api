/* date = May 3rd 2023 9:33 pm */

#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#ifdef GLM_VERSION
using namespace glm;
typedef vec2 v2;
typedef ivec2 v2i;
typedef vec3 v3;
typedef vec4 v4;
#define PI glm::pi<float>()
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

#endif //GLM_VERSION

#define EPSILON 0.001f
#define FULL_CIRCLE 2.0f * PI
#define QUARTER_CIRCLE (PI / 2.0f)
#define MAX_POLAR_ROTATION (QUARTER_CIRCLE - EPSILON)
#define UP v3(0, 1, 0)


#define squared(x) (x*x)


struct Spherical_Coordinates {
    f32 radius;
    f32 azimuth;
    f32 polar;
};

inline bool zero_vector(v3 v) {
    bool result = 
        (abs(v.x) < EPSILON) &&
        (abs(v.y) < EPSILON) &&
        (abs(v.z) < EPSILON);
    
    return result;
}

inline bool zero_vector(v2 v) {
    bool result = 
        (abs(v.x) < EPSILON) &&
        (abs(v.y) < EPSILON);
    
    return result;
}

inline f32 length(v3 a) {
    f32 result = sqrtf(dot(a, a));
    
    return result;
}


inline f32 length(v2 a) {
    f32 result = sqrtf(dot(a, a));
    
    return result;
}


void cartesian_to_spherical(v3 pos, f32 *_radius, f32 *_azimuth, f32 *_polar) {
    f32 radius = length(pos);
    f32 azimuth = atan(pos.y / pos.z);
    f32 polar = atan(length(v2(pos.x, pos.y)) / pos.z);
    
    *_radius = radius;
    *_azimuth = azimuth;
    *_polar = polar;
}


// NOTE(lmk): glm ortho -- (left, right, bottom, top, near, far)
// NOTE(lmk): glm lookAt -- (eye, center, up)


v2 screen_to_ndc(v2 screen_pos, int screen_width, int screen_height) {
    v2 result;
    result.x = ((screen_pos.x / screen_width)) * 2 - 1;
    result.y = ((screen_pos.y / screen_height)) * 2 - 1;
    return result;
}


f32 screen_x_to_ndc(f32 screen_pos, int screen_width) {
    f32 result = ((screen_pos / screen_width)) * 2 - 1;
    return result;
}


f32 screen_y_to_ndc(f32 screen_pos, int screen_height) {
    f32 result = ((screen_pos / screen_height)) * 2 - 1;
    return result;
}


struct Basis {
    v3 front;
    v3 right;
    v3 up;
};


void basis_from_front(Basis *basis, v3 front) {
    basis->front = normalize(front);
    basis->right = normalize(cross(basis->front, UP));
    basis->up = normalize(cross(basis->right, basis->front));
}


mat4 rotate(Basis *basis) {
    mat4 result;
    result[0] = v4(basis->right.x, basis->up.x, basis->front.x, 0);
    result[1] = v4(basis->right.y, basis->up.y, basis->front.y, 0);
    result[2] = v4(basis->right.z, basis->up.z, basis->front.z, 0);
    result[3] = v4(0, 0, 0, 1);
    return result;
}


v3 project(v3 a, v3 b) {
    v3 result = (dot(a, b) / squared(length(b))) * b;
    
    return result;
}


v3 project_onto_plane(v3 vector, v3 plane_normal) {
    v3 result = vector - project(vector, plane_normal);
    
    return result;
}

#endif //VECTOR_MATH_H
