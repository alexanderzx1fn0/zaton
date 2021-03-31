#version 330 core

layout (location = 0 ) in vec3 VertexPosition;

uniform mat4 MV;
uniform mat4 Proj;

void main()
{
    gl_Position = Proj * MV * vec4(VertexPosition, 1.0);
}
