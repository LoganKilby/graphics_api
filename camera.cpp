struct Camera_Orientation {
    Basis basis;
    v3 position;
};


struct Orbit_Camera_Orientation {
    Basis basis;
    f32 radius;
    f32 azimuth;
    f32 polar;
};


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