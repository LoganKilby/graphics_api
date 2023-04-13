/* date = April 8th 2023 3:24 pm */

#ifndef APP_H
#define APP_H

#include "arena.h"

struct Application_State {
    bool initialized;
    u32 vbo;
    
    Memory_Arena scratch_arena;
};

#endif //APP_H
