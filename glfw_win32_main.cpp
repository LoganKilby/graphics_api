#include "D:\Library\glm\glm\glm.hpp"
#include "D:\Library\glm\glm\gtc\constants.hpp"
#include "D:\Library\glm\glm\gtc\matrix_transform.hpp"
#include "defines.h"
#include "stdio.h" // for console output
#define APP_MEMORY_SIZE kilobytes(16)
#include "win32_platform.h"
#include "GL/glew.h"
#include "GL/wglew.h"
#include "opengl_utility/debug.h"
#include "D:/Library/glfw-3.3.8/include/GLFW/glfw3.h"
#define os_read_entire_file(file_name, bytes_read) win32_read_entire_file(file_name, bytes_read)
#include "app.h"
#include "app.cpp"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    GLFWwindow *window;
    {
        TIMED_BLOCK;
        window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "win32_glfw", 0, 0);
    }
    
    glfwMakeContextCurrent(window);
    
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
    
    Input_State input_state = {};
    
    while(!glfwWindowShouldClose(window)) {
        POINT cursor_pos;
        GetCursorPos(&cursor_pos);
        
        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
        input_state.mouse_pos.x = (int)cursor_x;
        input_state.mouse_pos.y = (int)cursor_y;
        
        update_and_render(&app_memory, &input_state);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

