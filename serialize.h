/* date = June 23rd 2023 10:08 am */

#ifndef SERIALIZE_H
#define SERIALIZE_H


#define SERIALIZATION_VERSION 0

enum Serialization_Version {
    
};


// TODO(lmk): Multiple scenes or 'levels' could be stored
enum Serializable_Types {
    S_Type_Entity = 0,
    S_Type_Orbit_Camera,
    S_Type_Skip,
    
    S_Type_Count
};


struct Blob_Index {
    Serializable_Types type;
    int count;
    uint64_t value_offset; // NOTE(lmk): offset from blob ptr, not header
    size_t size; // per element
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


#define BLOB_PADDING sizeof(u8)

void create_blob(Blob_Context *context) {
    *context = {};
    
    size_t base_size = sizeof(Blob_Header) + sizeof(Blob_Index) + BLOB_PADDING;
    context->memory = dynamic_alloc(base_size);
    
    if(context->memory) {
        context->size = base_size;
        memset(context->memory, 0, base_size);
        context->header = (Blob_Header *)context->memory;
        context->header->version = SERIALIZATION_VERSION;
        
        context->indices = (Blob_Index *)((u8 *)context->memory + sizeof(Blob_Header));
        context->indices->type = Serializable_Types::S_Type_Skip;
        context->header->index_count = 1;
        
        context->blob = context->indices + sizeof(Blob_Index);
        context->header->blob_size = BLOB_PADDING;
    }
    
    // TODO(lmk): Realistically I know ahead of time what is getting serialized... I should just allocate how much memory I need
    // ahead of time. The reason I didn't do this initially was because I anticipated that the data I'm
    // serializing will change frequently.
}


bool load_blob(Blob_Context *context, char *path) {
    *context = {};
    
    size_t bytes_read;
    context->memory = cfile_read(path, &bytes_read);
    
    if(bytes_read) {
        context->header = (Blob_Header *)context->memory;
        context->size = context->header->blob_size + sizeof(Blob_Header) + (sizeof(Blob_Index) * context->header->index_count);
        context->indices = (Blob_Index *)((u8 *)context->header + sizeof(Blob_Header));
        context->blob = (u8 *)context->indices + sizeof(Blob_Index) * context->header->index_count;
        
        return true;
    }
    
    return false;
}


// NOTE(lmk): This is okay for now since holding all of the scene data in-memory is not a problem. If I have a really large scene, maybe
// incrementally writing to an archive or something might be better. Like flush the blob buffer to the archive when it gets large enough
// or something?
// TODO(lmk): I'm growing two buffers independently that are actually contiguous in memory. It may make more sense to grow them separately
// and then concatenate them at the end with a single realloc, avoiding potentially many calls to realloc and memmove
Blob_Index *grow_blob(Blob_Context *context, size_t size) {
    if(context->header == 0) {
        fail("Forgot to initialize blob before serialization");
        return 0;
    }
    
    size_t realloc_offset = context->size;
    size_t bytes_allocated = size + sizeof(Blob_Index);
    size_t new_index_value_offset = context->header->blob_size;
    
    context->header->blob_size += size;
    context->size += bytes_allocated;
    
    context->memory = dynamic_realloc(context->memory, context->size);
    memset((u8 *)context->memory + realloc_offset, 0, bytes_allocated);
    
    context->blob = memmove((u8 *)context->blob + sizeof(Blob_Index), context->blob, context->header->blob_size);
    
    Blob_Index *result = &context->indices[context->header->index_count++];
    result->value_offset = new_index_value_offset;
    
    return result;
}


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


void serialize_entity(Blob_Context *context, Entity *data, int count) {
    size_t serialized_type_size = sizeof(S_Entity);
    
    // NOTE(lmk): using a transient alloc because the incoming data is contiguous, but total size is unknown
    S_Entity *s_entity_data = (S_Entity *)transient_alloc(serialized_type_size * count);
    memset(s_entity_data, 0, serialized_type_size * count);
    
    for(int i = 0; i < count; ++i) {
        s_entity_data[i].resource_id = data[i].resource_id;
        s_entity_data[i].position = data[i].position;
        s_entity_data[i].scale = data[i].scale;
        s_entity_data[i].basis = data[i].basis;
    }
    
    size_t data_size = serialized_type_size * count;
    Blob_Index *index = grow_blob(context, data_size);
    index->count = count;
    index->type = Serializable_Types::S_Type_Entity;
    index->size = serialized_type_size;
    memcpy((u8 *)context->blob + index->value_offset, (u8 *)data, data_size);
    
    transient_reset_last_alloc();
}


void serialize_orbit_camera(Blob_Context *context, Orbit_Camera *camera) {
    size_t serialized_type_size = sizeof(S_Orbit_Camera);
    
    S_Orbit_Camera s_orbit_camera = {};
    
    s_orbit_camera.attached_entity_id = camera->attached_entity_id;
    s_orbit_camera.basis = camera->basis;
    s_orbit_camera.position = camera->position;
    s_orbit_camera.target = camera->target;
    s_orbit_camera.look_speed = camera->look_speed;
    s_orbit_camera.zoom_speed = camera->zoom_speed;
    s_orbit_camera.pan_speed = camera->pan_speed;
    
    Blob_Index *bi = grow_blob(context, serialized_type_size);
    bi->count = 1;
    bi->type = Serializable_Types::S_Type_Orbit_Camera;
    bi->size = serialized_type_size;
    memcpy((u8 *)context->blob + bi->value_offset, (u8 *)&s_orbit_camera, serialized_type_size);
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


void deserialize_skip(void *data, Scene *scene) {
    return;
}


typedef void (*Deserialize_Ptr)(void *, Scene *);

// table[version][type]
Deserialize_Ptr deserialize_function_table[1][Serializable_Types::S_Type_Count] = {
    // Version 0
    {
        deserialize_entity_v0,
        deserialize_orbit_camera_v0,
        deserialize_skip,
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
            void *data = (u8 *)context->blob + bi->value_offset + (bi->size * data_index);
            _deserialize(data, scene);
        }
    }
}


#endif //SERIALIZE_H
