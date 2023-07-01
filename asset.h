/* date = June 30th 2023 5:55 pm */

#ifndef ASSET_H
#define ASSET_H

// need a reference counted list of all resources
// - need a list of all resources -- is this list generated? -- the list is implicit in the folder structure... need list without parsing
//   through the file system. there must be a generate resource list button
// - use meow hash
#pragma warning(disable : 4324)
#include "../../external/meow_hash/meow_hash_x64_aesni.h"

#define MAXIMUM_RESOURCE_COUNT 256 // arbitrary

u32 hash_u32(void *data, int size) {
    meow_u128 hash = MeowHash(MeowDefaultSeed, size, data);
    u32 result = MeowU32From(hash, 0);
    return result;
}

#endif //ASSET_H
