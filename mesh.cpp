struct Mesh_Vertex {
    v3 position;
    v3 normal;
    v3 uv;
};

struct Mesh {
    GL_Element_Buffer buffer;
    
    void create_buffer();
    void load(char *path);
};

void Mesh::create_buffer() {
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
}

void Mesh::load(char *path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        assert(0);
        // TODO(lmk): Load a default thing
        return;
    }
}


struct Application_Mesh_Info {
    char path[MAX_PATH];
};