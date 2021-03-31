#version 330 core

in float Transp;
in vec2 TexCoord;
// debug
in float time;
in vec3 cameraPosDebug;
in float ParticleLifetimeD;

uniform sampler2D ParticleTex;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);
    //FragColor.xyz = vec3(ParticleLifetimeD, 0.0, 0.0);
    FragColor.a *= Transp;
}
