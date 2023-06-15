#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 a_uv;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(pos.xy, 0.0, 1.0);
    a_uv = uv;
}
