#include "camera.h"

//
// Orbit camera
//

void rotate_orbit_camera_azimuth(Orbit_Camera_Orientation *camera, f32 radians) {
    camera->azimuth += radians;
    camera->azimuth = fmodf(camera->azimuth, FULL_CIRCLE);
    
    // NOTE(lmk): keeping the angle between [0, 2PI]
    if(camera->azimuth < 0) camera->azimuth = FULL_CIRCLE + camera->azimuth;
}


void rotate_orbit_camera_polar(Orbit_Camera_Orientation *camera, f32 radians) {
    camera->polar += radians;
    camera->polar = clamp(camera->polar, -MAX_POLAR_ROTATION, MAX_POLAR_ROTATION);
}


v3 orbit_camera_eye(Orbit_Camera_Orientation *camera, v3 target) {
    f32 sin_azimuth = sin(camera->azimuth);
    f32 cos_azimuth = cos(camera->azimuth);
    f32 sin_polar = sin(camera->polar);
    f32 cos_polar = cos(camera->polar);
    
    v3 result = v3(target.x + camera->radius * cos_polar * cos_azimuth,
                   target.y + camera->radius * sin_polar,
                   target.z + camera->radius * cos_polar * sin_azimuth);
    
    return result;
}


void rotate_orbit_camera(Orbit_Camera_Orientation *camera, v2 mouse_delta) {
    f32 adjusted_mouse_diff_x = Platform.mouse_diff.x * camera->look_speed * Platform.delta_time;
    f32 adjusted_mouse_diff_y = Platform.mouse_diff.y * camera->look_speed * Platform.delta_time;
    rotate_orbit_camera_azimuth(camera, radians(adjusted_mouse_diff_x));
    rotate_orbit_camera_polar(camera, radians(-adjusted_mouse_diff_y));
}


void zoom_orbit_camera(Orbit_Camera_Orientation *camera, f32 zoom_distance) {
    assert((zoom_distance == -1) || (zoom_distance == 1));
    camera->radius -= zoom_distance * camera->zoom_speed * Platform.delta_time;
}


mat4 lookAt_orbit_camera(Orbit_Camera_Orientation *camera, v3 target) {
    v3 camera_pos = orbit_camera_eye(camera, target);
    mat4 result = lookAt(camera_pos, target, UP);
    return result;
}


//
// Fly camera
//


void rotate_fly_camera(Fly_Camera_Orientation *camera, v2 mouse_delta) {
    camera->yaw += mouse_delta.x * camera->look_speed * Platform.delta_time;
    camera->pitch += mouse_delta.y * camera->look_speed * Platform.delta_time;
    camera->pitch = clamp(camera->pitch, -89.0f, 89.0f);
    
    v3 new_camera_front = camera->basis.front;
    new_camera_front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
    new_camera_front.y = sin(radians(camera->pitch));
    new_camera_front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
    new_camera_front = normalize(new_camera_front);
    basis_from_front(&camera->basis, new_camera_front);
}


void translate_fly_camera(Fly_Camera_Orientation *camera) {
    v3 camera_move = v3(0);
    if(glfwGetKey(Platform.window, GLFW_KEY_W) == GLFW_PRESS)
        camera_move += camera->basis.front;
    if(glfwGetKey(Platform.window, GLFW_KEY_S) == GLFW_PRESS)
        camera_move -= camera->basis.front;
    if(glfwGetKey(Platform.window, GLFW_KEY_A) == GLFW_PRESS)
        camera_move -= normalize(cross(camera->basis.front, camera->basis.up));
    if(glfwGetKey(Platform.window, GLFW_KEY_D) == GLFW_PRESS)
        camera_move += normalize(cross(camera->basis.front, camera->basis.up));
    
    if(!zero_vector(camera_move))
        camera->position += normalize(camera_move) * camera->move_speed * Platform.delta_time;
}


inline mat4 lookAt_fly_camera(Fly_Camera_Orientation *camera) {
    mat4 result = lookAt(camera->position, camera->position + camera->basis.front, UP);
    return result;
}