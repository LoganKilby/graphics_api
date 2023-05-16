#include "D:\Library\glm\glm\glm.hpp"
#include "D:\Library\glm\glm\gtc\constants.hpp"
#include "D:\Library\glm\glm\gtc\matrix_transform.hpp"

#include "win32_platform.h"
#include "app.h"
#include "app.cpp"

int main() {
    Platform_Stuff platform = {};
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    {
        TIMED_BLOCK;
        platform.window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "win32_glfw", 0, 0);
    }
    
    glfwMakeContextCurrent(platform.window);
    
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    }
    
    //glfwSetWindowSizeCallback();
    //glfwSetWindowFramebufferSizeCallback();
    //glfwSetKeyCallback();
    
    //HINSTANCE dll_handle;
    //Update_And_Render_Ptr update_and_render;
    //load_app_dll(&dll_handle, &update_and_render);
    
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, sizeof(Application_State), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
#if DEBUG
    assert(SetCurrentDirectory(DEBUG_WORKING_DIR));
#endif
    
    float last_frame_time = 0;
    while(!glfwWindowShouldClose(platform.window)) {
        float frame_time = (float)glfwGetTime();
        platform.delta_time = frame_time - last_frame_time;
        last_frame_time = frame_time;
        
        update_and_render(&app_memory, &platform);
        glfwSwapBuffers(platform.window);
        glfwPollEvents();
    }
}

