#version 330 core
layout (location = 0) in vec3 va_pos;
layout (location = 1) in vec2 va_uv;

uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;

out vec2 pass_uv;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(va_pos, 1.0);
	pass_uv = va_uv;
}