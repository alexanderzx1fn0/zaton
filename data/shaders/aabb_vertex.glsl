#version 330 core
layout(location = 0 ) in vec4 position;

uniform mat4 uViewProjM;
uniform mat4 uModelM;

void main()
{
    gl_Position = uViewProjM * uModelM * position;
}
