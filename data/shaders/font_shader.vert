#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;
layout (location = 3) in float time;

out vec2 a_uv;
out float a_t;
out vec3 a_color;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(pos.xy, 0.0, 1.0);
	a_color = color;
    a_uv = uv;
	a_t = time;
}
