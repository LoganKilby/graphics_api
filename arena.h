/* date = April 12th 2023 4:16 pm */

#ifndef ARENA_H
#define ARENA_H

#include "string.h" // memset
#define dynamic_alloc malloc
#define dynamic_realloc realloc

struct Memory {
    void *base_address;
    u64 size;
};


struct Memory_Arena {
    Memory memory;
    u64 allocated;
    u64 last_alloc; // # of bytes allocated from previous allocation
};


struct Debug_Memory_Allocation {
    char *caller;
    size_t size;
};

#define STRINGIFY(s) #s

Memory_Arena create_arena_local(void *memory, u64 size) {
    Memory_Arena result = {};
    result.memory.base_address = memory;
    result.memory.size = size;
    result.allocated = 0;
    
    return result;
}


// NOTE(lmk): Allocates if enough memory, no bookkeeping. "Freed" every frame
void *scratch_alloc(Memory_Arena *arena, u64 size) {
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


void stack_alloc(Memory_Arena *arena, u64 size) {
    // I haven't totally realized a compelling use case for a stack allocator yet
}


#endif //ARENA_H
