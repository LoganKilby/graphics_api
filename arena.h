/* date = April 12th 2023 4:16 pm */

#ifndef ARENA_H
#define ARENA_H

#include "string.h" // memset

struct Memory {
    void *base_address;
    u64 size;
};


struct Memory_Arena {
    Memory memory;
    u64 allocated;
    u64 last_alloc; // # of bytes allocated from previous allocation
    b32 locked;
};


#define dynamic_alloc malloc
#define dynamic_realloc realloc

Memory_Arena create_arena_local(void *memory, u64 size) {
    Memory_Arena result = {};
    result.memory.base_address = memory;
    result.memory.size = size;
    result.allocated = 0;
    
    return result;
}


// NOTE(lmk): Allocates if enough memory, no bookkeeping
void *stack_alloc(Memory_Arena *arena, u64 size) {
    assert(arena->memory.base_address);
    u8 *result = 0;
    
    if(size < (arena->memory.size - arena->allocated)) {
        result = (u8 *)arena->memory.base_address + arena->allocated;
        arena->allocated += size;
        arena->last_alloc = size;
        memset(result, 0, size);
    } else {
        assert(0); // NOTE(lmk): not enough memory
    }
    
    return result;
}


void reset_last_alloc(Memory_Arena *arena, size_t size) {
    assert(arena->last_alloc > 0); // You must allocate before resetting
    assert(arena->last_alloc == size); // last allocation size mismatch
    
    arena->allocated -= arena->last_alloc;
    arena->last_alloc = 0;
}


#endif //ARENA_H
