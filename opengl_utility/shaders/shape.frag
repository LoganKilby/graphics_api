R"(
#version 330 core
out vec4 output_fragment_color;

uniform vec4 u_color;

void main() {
	output_fragment_color = u_color;
}

)"