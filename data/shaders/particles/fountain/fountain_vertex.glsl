#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in float size;
layout (location = 3) in float time;
layout (location = 4) in float lifeTime;
layout (location = 5) in vec4 color;



uniform vec3 gAccel;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float gTime;
uniform float glViewportHeight;

out vData
{
    float size;
    vec4 color;
}vertex;




void main()
{
    float t = gTime - time;
    vec3 posL = position;
    //float pointSize = size;
    
    // Constant acceleration.
    posL = posL + velocity*t;

    vertex.size = size;


    vertex.color = color * vec4((1.0f - (t / lifeTime)));

    gl_Position = ModelViewMatrix * vec4(posL,1.0);
}
