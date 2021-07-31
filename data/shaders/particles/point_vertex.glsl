#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in float size;
layout (location = 3) in float time;
layout (location = 4) in float lifeTime;
layout (location = 5) in float mass;
layout (location = 6) in vec3 color;



uniform vec3 eyePos;
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
		float pointSize = size;

    // Rotate the particles about local space.
    float sine = sin(0.5*mass*t);
    float cosine = cos(0.5*mass*t);
    float x = posL.x*cosine + posL.y*-sine;
    float y = posL.x*sine + posL.y*cosine;
    
    // Oscillate particles up and down.
    float s = sin(6.0*t);
    posL.x = x;
    posL.y = y + mass*s;
    
    // Constant acceleration.
    posL = posL + velocity*t + 0.5f * gAccel * t * t;

    pointSize += 8.0 * t * t;
    float d = distance(posL, eyePos);
    vertex.size = 1.;//glViewportHeight/(1.0 + 8.0 * d);

    vertex.color = vec4((1.0f - (t / lifeTime)));

    gl_Position = ModelViewMatrix * vec4(posL,1.0);
}
