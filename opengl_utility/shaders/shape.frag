R"(
#version 330 core
out vec4 output_fragment_color;

uniform vec4 uniform_fragment_color;

void main() {
	output_fragment_color = uniform_fragment_color;
}

)"