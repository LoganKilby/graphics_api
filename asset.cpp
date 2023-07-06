#include "asset.h"


GL_Element_Buffer create_mesh_buffer() {
    GL_Element_Buffer buffer = {};
    
    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);
    
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo); // TODO(lmk): may not need to bind this
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh_Vertex), (void *)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh_Vertex), (void *)offsetof(Mesh_Vertex, normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh_Vertex), (void *)offsetof(Mesh_Vertex, uv));
    
    glBindVertexArray(0);
    
    return buffer;
}


static void assimp_process_mesh_node(Model *model, aiNode *node, const aiScene *scene) {
    for(u32 node_index = 0; node_index < node->mNumMeshes; ++node_index) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[node_index]];
    }
}


static u32 assimp_count_meshes(aiNode *node, const aiScene *scene) {
    u32 mesh_count = node->mNumMeshes;
    
    for(u32 child_index = 0; child_index < node->mNumChildren) {
        mesh_count += assimp_count_meshes(node, scene);
    }
    
    return mesh_count;
}

Model *load_model(Game_Assets *assets, Memory_Arena *arena, char *path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        assert(0);
        // TODO(lmk): Load a default thing
        return 0;
    }
    
    // load data
    
    Model *result = 0;
    
    aiNode *node = scene->mRootNode;
    result->mesh_count = assimp_count_meshes(node, scene);
    
    
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
    
    
    return result;
}
