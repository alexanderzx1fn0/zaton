#version 330 core
/*
layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;
layout(location = 2 ) in vec3 tangent;
layout(location = 3 ) in vec3 bitangent;
layout(location = 4 ) in vec2 texcoord;
*/
layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;
layout(location = 2 ) in vec2 texcoord;

uniform mat4 uViewProjM;
uniform mat4 modelView;
uniform mat4 uModelM;

out vec2 uv;

void main()
{
    uv = texcoord;
    gl_Position = uViewProjM * uModelM * vec4(position, 1.0f);
}
