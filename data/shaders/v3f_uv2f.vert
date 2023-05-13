#version 330 core
layout (location = 0) in vec3 va_pos;
layout (location = 1) in vec2 va_uv;

uniform mat4 transform;

out vec2 pass_uv;

void main() {
	gl_Position = transform * vec4(va_pos, 1.0);
	pass_uv = va_uv;
}