#version 330 core
layout (location = 0) in vec3 va_pos;
layout (location = 1) in vec2 va_uv;

out vec2 pass_uv;

void main() {
	gl_Position = vec4(va_pos, 1.0);
	pass_uv = va_uv;
}