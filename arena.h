/* date = April 12th 2023 4:16 pm */

#ifndef ARENA_H
#define ARENA_H

#include "string.h" // memset

struct Memory_Arena {
    void *base_address;
    u64 size;
    u64 allocated;
    u64 last_alloc; // # of bytes allocated from previous allocation
};

global Memory_Arena transient_arena;


// TODO(lmk): Not a lot of use cases for this, so maybe don't need to create my own...
#define dynamic_alloc malloc
#define dynamic_realloc realloc


Memory_Arena create_arena_local(void *memory, u64 size) {
    Memory_Arena result = {};
    result.base_address = memory;
    result.size = size;
    result.allocated = 0;
    
    return result;
}


// NOTE(lmk): resets the arena without deallocating any memory
inline void reset_arena(Memory_Arena* arena) { arena->allocated = 0; }

// NOTE(lmk): Allocates if enough memory, no bookkeeping
void *transient_alloc(u64 size) {
    assert(transient_arena.base_address);
    u8 *result = 0;
    
    if(size < (transient_arena.size - transient_arena.allocated)) {
        result = (u8 *)transient_arena.base_address + transient_arena.allocated;
        transient_arena.allocated += size;
        transient_arena.last_alloc = size;
        memset(result, 0, size);
    } else {
        assert(0); // NOTE(lmk): not enough memory
    }
    
    return result;
}


void transient_reset_last_alloc() {
    transient_arena.size -= transient_arena.last_alloc;
    transient_arena.last_alloc = 0;
}


#endif //ARENA_H
