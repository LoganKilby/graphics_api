#include "D:\Library\glm\glm\glm.hpp"
#include "D:\Library\glm\glm\gtc\constants.hpp"
#include "D:\Library\glm\glm\gtc\matrix_transform.hpp"

#include "win32_platform.h"
#include "tweak.h"
#include "camera.cpp"

#include "app.h"
#include "app.cpp"

global Platform_Stuff Platform;

void glfw_window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        // The window gained input focus
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        Platform.mouse_diff = {};
        Platform.mouse_pos = v2(mouse_x, mouse_y);
    }
    else
    {
        // The window lost input focus
    }
}

void glfw_scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
    // TODO(lmk): event?
    Platform.mouse_scroll_delta = (f32)y_offset;
}

int main() {
    Platform = {};
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    {
        TIMED_BLOCK;
        Platform.window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "win32_glfw", 0, 0);
    }
    
    glfwMakeContextCurrent(Platform.window);
    glfwSetInputMode(Platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    }
    
    glfwSetWindowFocusCallback(Platform.window, glfw_window_focus_callback);
    glfwSetScrollCallback(Platform.window, glfw_scroll_callback);
    //glfwSetWindowSizeCallback();
    //glfwSetWindowFramebufferSizeCallback();
    //glfwSetKeyCallback();
    
    Memory_Arena app_memory = {};
    app_memory.size = APP_MEMORY_SIZE;
    app_memory.base_address = VirtualAlloc(0, sizeof(Application_State), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
#if DEBUG
    assert(SetCurrentDirectory(DEBUG_WORKING_DIR));
#endif
    
    double cursor_x, cursor_y;
    glfwGetCursorPos(Platform.window, &cursor_x, &cursor_y);
    Platform.mouse_pos = v2(cursor_x, cursor_y);
    
    float last_frame_time = 0;
    while(!glfwWindowShouldClose(Platform.window)) {
        float frame_time = (float)glfwGetTime();
        Platform.delta_time = frame_time - last_frame_time;
        last_frame_time = frame_time;
        
        glfwGetCursorPos(Platform.window, &cursor_x, &cursor_y);
        Platform.mouse_diff = v2(cursor_x - Platform.mouse_pos.x, Platform.mouse_pos.y - cursor_y);
        Platform.mouse_pos = v2(cursor_x, cursor_y);
        
        update_and_render(&app_memory, &Platform);
        
        glfwSwapBuffers(Platform.window);
        
        Platform.mouse_scroll_delta = 0;
        glfwPollEvents();
    }
}
