#version 330 core
in vec2 a_uv;
in float a_t;
in vec3 a_color;
out vec4 color;

uniform sampler2D u_texture;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, a_uv).r);
    color = vec4(a_color.rgb, 1.0) * (1 - a_t) * sampled;
}  
