#include "shared.h"
#include "D:/Library/glfw-3.3.8/include/GLFW/glfw3.h"

// NOTE(lmk): Having trouble setting up an opengl context for fixed pipeline opengl

int main() {
    glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    glewInit();
    GLFWwindow *window;
    {
        TIMED_BLOCK;
        window = glfwCreateWindow(640, 480, "fixed function", 0, 0);
    }
    
    
    HINSTANCE dll_handle;
    Update_And_Render_Ptr update_and_render;
    load_app_dll(&dll_handle, &update_and_render);
    
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, APP_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    global_input_state = {};
    
    while(!glfwWindowShouldClose(window)) {
        win32_hot_reload(&dll_handle, &update_and_render);
        update_and_render(&app_memory, &global_input_state);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}