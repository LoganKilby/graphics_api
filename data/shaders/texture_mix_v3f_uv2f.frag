#version 330 core

out vec4 frag_color;

in vec2 pass_uv;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	frag_color = mix(texture(texture0, pass_uv), texture(texture1, pass_uv), 0.2);
}