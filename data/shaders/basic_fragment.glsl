#version 330 core

uniform sampler2D uDiffuse;
uniform vec3 color;
out vec4 fragColor;
in vec2 uv;
in vec3 n;

void main()
{
    //fragColor = texture(uDiffuse, uv.xy);
    fragColor = vec4(normalize(n), 1.0);
}
