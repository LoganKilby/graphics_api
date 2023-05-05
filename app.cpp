#include "shared.h"
#include "arena.h"
#include "app.h"
#include "opengl_utility/core.h"
#include "opengl_utility/debug.h"

extern "C" __declspec(dllexport) void update_and_render(Memory_Arena *platform_memory, Input_State *input) {
    Application_State *app_state = (Application_State *)platform_memory->base_address;
    assert(sizeof(Application_State) < APP_MEMORY_SIZE); // NOTE(lmk): Increase app memory size
    
    if(!app_state->initialized) {
        scratch_arena = create_arena_local((u8 *)platform_memory->base_address + sizeof(Application_State),
                                           APP_MEMORY_SIZE - sizeof(Application_State));
        glClearColor(0, 1, 0, 0);
        app_state->initialized = true;
    }
    
    // TODO(lmk): begin frame
    scratch_arena.allocated = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    
}