#version 330 core

uniform vec3 Color;
in vec3 n;
in vec2 uv;

struct LightInfo
{
    vec4 Position;
    vec3 Intensitiy;
};

uniform LightInfo lights[5];

layout( location = 0 ) out vec4 FragColor;

void main() {
    FragColor = vec4(Color, 1.0);
}
