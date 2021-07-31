#version 330 core

in vec2 TexCoord;

uniform sampler2D SpriteTex;

layout( location = 0 ) out vec4 FragColor;
in vec4 vertex_color;


void main()
{
    FragColor = vertex_color * texture(SpriteTex, TexCoord); 
}
