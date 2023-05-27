/* date = May 27th 2023 5:48 pm */

#ifndef CAMERA_H
#define CAMERA_H


enum Camera_Type {
    Fly,
    Orbit,
};


// NOTE(lmk): Position is relative to target
struct Orbit_Camera_Orientation {
    f32 radius;
    f32 azimuth;
    f32 polar;
    f32 look_speed;
    f32 zoom_speed;
};


struct Fly_Camera_Orientation {
    Basis basis;
    v3 position;
    f32 move_speed;
    f32 look_speed;
    f32 yaw;
    f32 pitch;
};


#endif //CAMERA_H
