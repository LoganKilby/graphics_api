#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_texture;
//uniform vec3 u_text_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, TexCoords).r);
    color = vec4(1.0, 1.0, 1.0, 1.0) * sampled; // Could add custom text color..
}  
