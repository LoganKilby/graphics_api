/* date = May 27th 2023 5:48 pm */

#ifndef CAMERA_H
#define CAMERA_H


enum Camera_Type {
    Fly,
    Orbit,
};


// NOTE(lmk): Position is relative to target
struct Orbit_Camera {
    Basis basis;
    Spherical_Coordinates position;
    v3 target;
    Basis target_basis;
    f32 look_speed;
    f32 zoom_speed;
    f32 pan_speed;
};


struct Fly_Camera {
    Basis basis;
    v3 position;
    f32 move_speed;
    f32 look_speed;
    f32 yaw;
    f32 pitch;
};

#endif //CAMERA_H
