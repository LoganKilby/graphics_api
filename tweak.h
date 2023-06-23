/* date = May 15th 2023 9:45 pm */

#ifndef TWEAK_H
#define TWEAK_H

#define DEFAULT_CAMERA_POS v3(0, 0, 10)
#define DEFAULT_PLAYER_SPEED 8.0f
#define DEBUG_SCENE_NAME "persistent_scene.scene"
#define TRANSIENT_ARENA_SIZE kilobytes(16)
#define APP_MEMORY_SIZE kilobytes(16)

#define DEFAULT_ORBIT_CAMERA_LOOK_SPEED 4.0f
#define DEFAULT_ORBIT_CAMERA_ZOOM_SPEED 24.0f
#define DEFAULT_ORBIT_CAMERA_PAN_SPEED 1.0f
#define DEFAULT_FLY_CAMERA_LOOK_SPEED 0.1f
#define DEFAULT_FLY_CAMERA_MOVE_SPEED 2.0f

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f
#define FOV radians(45.0f)
#define WORLD_UP v3(0, 1, 0)

#endif //TWEAK_H
