#version 330 core
layout (location = 0) in vec3 va_pos;
layout (location = 1) in vec3 va_color;

out vec3 frag_va_color;

void main()
{
    gl_Position = vec4(va_pos.x, va_pos.y, va_pos.z, 1.0);
	frag_va_color = va_color;
}