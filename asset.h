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


struct Mesh_Vertex {
    v3 position;
    v3 normal;
    v3 uv;
};


struct Triangle_Mesh {
    GL_Element_Buffer buffer;
    u32 vertex_count;
    Mesh_Vertex *vertices;
    u32 index_count;
    u32 *indices;
};


struct Model {
    Triangle_Mesh *mesh;
};


enum Game_Asset_ID {
    GAI_Fox,
    
    GAI_Count
};


enum Game_Asset_Model_ID {
    GAMI_Fox,
    
    GAMI_Count
};


struct Game_Assets {
    Model game_meshes[Game_Asset_Model_ID::GAMI_Count];
};


#endif //ASSET_H
