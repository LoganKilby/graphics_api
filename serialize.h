/* date = June 23rd 2023 10:08 am */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#define SERIALIZATION_VERSION 0

// TODO(lmk): Multiple scenes or 'levels' could be stored
enum Serializable_Types {
    S_Type_Entity = 0,
    S_Type_Orbit_Camera,
    
    S_Type_Count
};


struct S_Entity {
    int resource_id;
    v3 position;
    v3 scale;
    Basis basis;
};


struct S_Orbit_Camera {
    int attached_entity_id;
    Basis basis;
    Spherical_Coordinates position;
    v3 target;
    f32 look_speed;
    f32 zoom_speed;
    f32 pan_speed;
};


struct Blob_Index {
    Serializable_Types type;
    int count;
    uint64_t value_offset; // NOTE(lmk): offset from blob ptr, not header
    size_t element_size;
};


struct Blob_Chunk {
    void *data;
    size_t size;
    Blob_Index *index;
};


// NOTE(lmk): If the header changes, all bets are off unless I add the Blob_Header to the Serializable_Types. This is because I decided
// to store the version in the blob header. I could prob store the version in the file name as an alternative.
struct Blob_Header {
    size_t blob_size;
    int version;
    int index_count;
};

// NOTE(lmk): This is an interface to the blob structure. Does NOT get written to disk.
struct Blob_Context {
    size_t size;
    void *memory; // NOTE(lmk): Effectively a Blob_Header ptr. It's the head of the buffer that is read to/from disk
    
    Blob_Header *header;
    Blob_Index *indices;
    void *blob;
};


void create_blob(Blob_Context *context, int index_count, size_t data_size, Memory_Arena *arena) {
    memset(context, 0, sizeof(Blob_Context));
    
    size_t buffer_size = data_size + sizeof(Blob_Header) + (sizeof(Blob_Index) * index_count);
    context->memory = scratch_alloc(arena, buffer_size);
    memset(context->memory, 0, buffer_size);
    
    context->size = buffer_size;
    context->header = (Blob_Header *)context->memory;
    context->indices = (Blob_Index *)((u8 *)context->header + sizeof(Blob_Header));
    context->blob = ((u8 *)context->indices + (sizeof(Blob_Index) * index_count));
}


bool load_blob(Blob_Context *context, char *path) {
    memset(context, 0, sizeof(Blob_Context));
    
    size_t bytes_read;
    context->memory = cfile_read(path, &bytes_read);
    
    if(bytes_read) {
        context->size = bytes_read;
        context->header = (Blob_Header *)context->memory;
        context->indices = (Blob_Index *)((u8 *)context->memory + sizeof(Blob_Header));
        context->blob = (u8 *)context->memory + sizeof(Blob_Header) + sizeof(Blob_Index) * context->header->index_count;
        
        return true;
    }
    
    return false;
}


// NOTE(lmk): This does not set up the indices
Blob_Chunk register_blob_chunk(Blob_Context *context, size_t size) {
    Blob_Chunk result = {};
    result.index = &context->indices[context->header->index_count++];
    result.index->value_offset = context->header->blob_size;
    result.data = (u8 *)context->blob + result.index->value_offset;
    result.size = size;
    
    context->header->blob_size += size;
    
    return result;
}


void serialize_entity(Blob_Context *context, Entity *data, int count) {
    size_t serialized_type_size = sizeof(S_Entity);
    size_t data_size = serialized_type_size * count;
    
    Blob_Chunk chunk = register_blob_chunk(context, data_size);
    chunk.index->type = Serializable_Types::S_Type_Entity;
    chunk.index->count = count;
    chunk.index->element_size = serialized_type_size;
    
    S_Entity *s_entity_buffer = (S_Entity *)chunk.data;
    for(int data_index = 0; data_index < count; ++data_index) {
        s_entity_buffer[data_index].resource_id = data[data_index].resource_id;
        s_entity_buffer[data_index].position = data[data_index].position;
        s_entity_buffer[data_index].scale = data[data_index].scale;
        s_entity_buffer[data_index].basis = data[data_index].basis;
    }
}


void serialize_orbit_camera(Blob_Context *context, Orbit_Camera *camera) {
    size_t serialized_type_size = sizeof(S_Orbit_Camera);
    
    Blob_Chunk chunk = register_blob_chunk(context, serialized_type_size);
    chunk.index->count = 1;
    chunk.index->type = Serializable_Types::S_Type_Orbit_Camera;
    chunk.index->element_size = serialized_type_size;
    
    S_Orbit_Camera *s_orbit_camera = (S_Orbit_Camera *)chunk.data;
    s_orbit_camera->attached_entity_id = camera->attached_entity_id;
    s_orbit_camera->basis = camera->basis;
    s_orbit_camera->position = camera->position;
    s_orbit_camera->target = camera->target;
    s_orbit_camera->look_speed = camera->look_speed;
    s_orbit_camera->zoom_speed = camera->zoom_speed;
    s_orbit_camera->pan_speed = camera->pan_speed;
}


void deserialize_entity_v0(void *data, Scene *scene) {
    Entity *entity = &scene->entities[scene->entity_count++];
    S_Entity *s_entity = (S_Entity *)data;
    
    entity->resource_id = s_entity->resource_id;
    entity->position = s_entity->position;
    entity->scale = s_entity->scale;
    entity->basis = s_entity->basis;
}


void deserialize_orbit_camera_v0(void *data, Scene *scene) {
    Orbit_Camera *camera = &scene->camera;
    S_Orbit_Camera *s_camera = (S_Orbit_Camera *)data;
    
    camera->attached_entity_id = s_camera->attached_entity_id;
    camera->basis = s_camera->basis;
    camera->position = s_camera->position;
    camera->target = s_camera->target;
    camera->look_speed = s_camera->look_speed;
    camera->zoom_speed = s_camera->zoom_speed;
    camera->pan_speed = s_camera->pan_speed;
}


typedef void (*Deserialize_Ptr)(void *, Scene *);

// table[version][type]
Deserialize_Ptr deserialize_function_table[1][Serializable_Types::S_Type_Count] = {
    // Version 0
    {
        deserialize_entity_v0,
        deserialize_orbit_camera_v0,
    },
};


void deserialize(Blob_Context *context, Scene *scene) {
    int version = context->header->version;
    
    for(int i = 0; i < context->header->index_count; ++i) {
        Blob_Index *bi = &context->indices[i];
        int data_count = bi->count;
        int data_type = bi->type;
        
        Deserialize_Ptr _deserialize = deserialize_function_table[version][data_type];
        for(int data_index = 0; data_index < data_count; ++data_index) {
            void *data = (u8 *)context->blob + bi->value_offset + (bi->element_size * data_index);
            _deserialize(data, scene);
        }
    }
}


void *get_blob_data(Blob_Context *context, int index) {
    void *result = ((u8 *)context->blob + context->indices[index].value_offset);
    
    return result;
}


#endif //SERIALIZE_H
