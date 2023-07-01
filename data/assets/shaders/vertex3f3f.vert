#version 330 core
layout (location = 0) in vec3 va_pos;
layout (location = 1) in vec3 va_color;

out vec3 pass_frag_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(va_pos.x, va_pos.y, va_pos.z, 1.0);
	pass_frag_color = va_color;
}