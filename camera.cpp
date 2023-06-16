#include "camera.h"

//
// Orbit camera
//


v3 orbit_camera_eye(Orbit_Camera *camera) {
    f32 sin_azimuth = sin(camera->position.azimuth);
    f32 cos_azimuth = cos(camera->position.azimuth);
    f32 sin_polar = sin(camera->position.polar);
    f32 cos_polar = cos(camera->position.polar);
    
    v3 result = v3(camera->target.x + camera->position.radius * cos_polar * cos_azimuth,
                   camera->target.y + camera->position.radius * sin_polar,
                   camera->target.z + camera->position.radius * cos_polar * sin_azimuth);
    
    return result;
}


void rotate_orbit_camera_azimuth(Orbit_Camera *camera, f32 radians) {
    camera->position.azimuth += radians;
    camera->position.azimuth = fmodf(camera->position.azimuth, FULL_CIRCLE);
    
    // NOTE(lmk): keeping the angle between [0, 2PI]
    if(camera->position.azimuth < 0) camera->position.azimuth = FULL_CIRCLE + camera->position.azimuth;
}


void rotate_orbit_camera_polar(Orbit_Camera *camera, f32 radians) {
    camera->position.polar += radians;
    camera->position.polar = clamp(camera->position.polar, -MAX_POLAR_ROTATION, MAX_POLAR_ROTATION);
}


void pan_orbit_camera(Orbit_Camera *camera, v2 mouse_diff) {
    if(!zero_vector(mouse_diff)) {
        assert(camera->pan_speed > 0);
        camera->target += camera->basis.right * -mouse_diff.x * camera->pan_speed * Platform.delta_time;
        
        // TODO(lmk): when the camera is oriented down the y axis, we cannot pan around the xz plane because of the way
        // we're multiplying the pan vector..
        v3 pan_vector = project_onto_plane(camera->basis.front, UP);
        camera->target +=  pan_vector * -mouse_diff.y * camera->pan_speed * Platform.delta_time;
    }
}


// NOTE(lmk): Spherical to cartesian
Spherical_Coordinates orbit_camera_spherical_position(v3 camera_pos, v3 target_pos) {
    v3 target_to_camera = camera_pos - target_pos;
    
    Spherical_Coordinates result = {};
    cartesian_to_spherical(target_to_camera, &result.radius, &result.azimuth, &result.polar);
    
    return result;
}


void rotate_orbit_camera(Orbit_Camera *camera, v2 mouse_delta) {
    if(!zero_vector(mouse_delta)) {
        f32 adjusted_mouse_diff_x = Platform.input_state.mouse_diff.x * camera->look_speed * Platform.delta_time;
        f32 adjusted_mouse_diff_y = Platform.input_state.mouse_diff.y * camera->look_speed * Platform.delta_time;
        rotate_orbit_camera_azimuth(camera, radians(adjusted_mouse_diff_x));
        rotate_orbit_camera_polar(camera, radians(-adjusted_mouse_diff_y));
        
        v3 camera_pos = orbit_camera_eye(camera);
        v3 new_camera_front = normalize(camera->target - camera_pos);
        basis_from_front(&camera->basis, new_camera_front);
    }
}


void zoom_orbit_camera(Orbit_Camera *camera, f32 zoom_distance) {
    assert(camera->zoom_speed > 0);
    camera->position.radius -= zoom_distance * camera->zoom_speed * Platform.delta_time;
}


mat4 lookAt_orbit_camera(Orbit_Camera *camera) {
    v3 camera_pos = orbit_camera_eye(camera);
    mat4 result = lookAt(camera_pos, camera->target, UP);
    return result;
}


// NOTE(lmk): another way to get the spherical coordinates relative to a target
void attach_orbit_camera(Orbit_Camera *camera, v3 target_pos, v3 target_front, f32 dist_from_target) {
    v3 camera_pos = target_pos + (UP * dist_from_target);
    
    v3 camera_front = normalize(target_pos - camera_pos);
    basis_from_front(&camera->basis, camera_front);
    
    Spherical_Coordinates orbit_pos;
    cartesian_to_spherical(camera_pos, &orbit_pos.radius, &orbit_pos.azimuth, &orbit_pos.polar);
    
    rotate_orbit_camera_polar(camera, orbit_pos.polar);
    rotate_orbit_camera_azimuth(camera, orbit_pos.azimuth);
    camera->position.radius = orbit_pos.radius;
}


void update_orbit_camera(Orbit_Camera *camera, v3 target) {
    camera->target = target;
    
    // if a ui window is hovered by the mouse, don't let mouse interact with the camera
    if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if(Platform.input_state.mouse_scroll_delta)
            zoom_orbit_camera(camera, Platform.input_state.mouse_scroll_delta);
        
        if(is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT) || is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if(!zero_vector(Platform.input_state.mouse_diff)) {
                rotate_orbit_camera(camera, Platform.input_state.mouse_diff);
                v3 camera_pos = orbit_camera_eye(camera);
                v3 camera_front = normalize(target - camera_pos);
                basis_from_front(&camera->basis, camera_front);
            }
        }
    }
}


//
// Fly camera
//


void rotate_fly_camera(Fly_Camera *camera, v2 mouse_delta) {
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


void translate_fly_camera(Fly_Camera *camera) {
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


inline mat4 lookAt_fly_camera(Fly_Camera *camera) {
    mat4 result = lookAt(camera->position, camera->position + camera->basis.front, UP);
    return result;
}