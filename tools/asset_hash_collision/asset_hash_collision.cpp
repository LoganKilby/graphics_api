#include <filesystem>
#include "../../defines.h"
#include "../../asset.h"
#include <iostream>

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "asset_hash_collision: error: no file path provided" << std::endl;
    }
    
    const char *root_asset_path = "../../data/assets";
    
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
    
    b32 asset_ids[MAXIMUM_RESOURCE_COUNT] = {};
    
    b32 collision_count = 0;
    std::string filename;
    
    for (auto const& iterator : recursive_directory_iterator(root_asset_path)) {
        
        if(iterator.is_regular_file()) {
            std::string fn = iterator.path().filename().string();
            u32 index = hash_u32((char *)fn.c_str(), (int)fn.length()) % MAXIMUM_RESOURCE_COUNT;
            
            std::cout << fn << " " << index << std::endl;
            
            if(index < countof(asset_ids)) {
                if(!asset_ids[index]) {
                    // valid hash
                    asset_ids[index] = index;
                } else {
                    ++collision_count;
                    std::cout << "WARNING: Asset hash collision: " << fn << std::endl;
                }
            } else {
                //std::cout << "ERROR: Asset hash index out of bounds" << std::endl;
                //return -1;
            }
        }
    }
    
    return collision_count ? -1 : 0;
}