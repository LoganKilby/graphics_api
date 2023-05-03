/* date = April 12th 2023 4:16 pm */

#ifndef ARENA_H
#define ARENA_H

#include "string.h" // memset

struct Memory_Arena {
    void *base_address;
    u64 size;
    u64 allocated;
};

Memory_Arena create_arena_local(void *memory, u64 size) {
    Memory_Arena result = {};
    result.base_address = memory;
    result.size = size;
    result.allocated = 0;
    
    return result;
}

global Memory_Arena scratch_arena = {};

// NOTE(lmk): Allocates if enough memory, no bookkeeping
void *scratch_allocate(u64 size) {
    u8 *result = 0;
    
    if(size < (scratch_arena.size - scratch_arena.allocated)) {
        result = (u8 *)scratch_arena.base_address + scratch_arena.allocated;
        scratch_arena.allocated += size;
        memset(result, 0, size);
    } else {
        assert(0); // NOTE(lmk): not enough memory
    }
    
    return result;
}

#endif //ARENA_H
