#version 330 core
out vec4 FragColor;

in vec3 frag_va_color;

uniform vec4 uniform_fragment_color;

void main() {
	FragColor = vec4(frag_va_color, 1.0);
}
