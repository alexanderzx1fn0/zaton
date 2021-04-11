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
uniform mat4 uNormalMatrix;


out vec2 uv;
out vec3 n;
out vec3 p;

void main()
{
    p = vec3(uModelM * vec4(position, 1.0));
    n = normalize(vec3(uNormalMatrix * vec4(normal, 0.0f)));
    uv = texcoord;
    gl_Position = uViewProjM * uModelM * vec4(position, 1.0f);
}
