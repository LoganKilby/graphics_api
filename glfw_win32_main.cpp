#include "shared.h"


// TODO(lmk): Having trouble setting up an opengl context for fixed pipeline opengl on my hardware...

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    glewInit();
    GLFWwindow *window;
    {
        TIMED_BLOCK;
        window = glfwCreateWindow(640, 480, "win32_glfw", 0, 0);
    }
    
    HINSTANCE dll_handle;
    Update_And_Render_Ptr update_and_render;
    load_app_dll(&dll_handle, &update_and_render);
    
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, APP_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    Input_State input_state = {};
    
    while(!glfwWindowShouldClose(window)) {
        debug_hot_reload_app_dll(&dll_handle, &update_and_render);
        update_and_render(&app_memory, &input_state);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}