#version 330 core
out vec4 frag_color;

in vec3 pass_frag_color;

void main() {
	frag_color = vec4(pass_frag_color, 1.0);
}