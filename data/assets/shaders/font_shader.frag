#version 330 core
in vec2 a_uv;
out vec4 color;

uniform sampler2D u_texture;
uniform vec3 u_color;
uniform float u_t;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, a_uv).r);
    color = vec4(u_color.rgb, 1.0) * (1 - u_t) * sampled;
}  
