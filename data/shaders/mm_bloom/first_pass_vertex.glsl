#version 330 core

layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;
layout(location = 2 ) in vec3 tangent;
layout(location = 3 ) in vec3 bitangent;
layout(location = 4 ) in vec2 texcoord;

uniform mat4 proj;
uniform mat4 viewModel;
uniform mat4 model;
uniform mat3 normalMatrix;

struct LightInfo
{
    vec4 Position;
    vec3 Intensity;
};

out vec3 n;
out vec3 p;
out vec2 uv;


void main()
{
    uv = vec2(tangent.x, tangent.y);

    p = vec3(model * vec4(position, 1.0));
    n = normalize(normalMatrix * normal);

    gl_Position = proj * viewModel * model * vec4(position, 1.0f);
}
