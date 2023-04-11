#include "platform.h"
#include <GL/gl.h>
#include "app.h"

extern "C" __declspec(dllexport) void update_and_render(Application_Memory memory) {
    glClearColor(0, 1, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

