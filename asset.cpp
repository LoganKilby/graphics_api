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


static void assimp_process_mesh_node(aiNode *node, const aiScene *scene) {
    
}

Model *load_model(Game_Assets *assets, Memory_Arena *arena, char *path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        assert(0);
        // TODO(lmk): Load a default thing
        return;
    }
    
    // load data
    
    aiNode *node = scene->mRootNode;
    
    Model *result = 0;
    return result;
}
