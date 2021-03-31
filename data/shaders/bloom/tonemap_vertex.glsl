#version 330 core

layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;
layout(location = 2 ) in vec3 tangent;
layout(location = 3 ) in vec3 bitangent;
layout(location = 4 ) in vec2 texcoord;

uniform mat4 proj;
uniform mat4 viewModel;
uniform mat4 model;

out vec3 n;
out vec2 uv;

void main()
{
    n = normal;
    uv = vec2(tangent.x, tangent.y);
    gl_Position = proj * viewModel * vec4(position, 1.0f);
}
