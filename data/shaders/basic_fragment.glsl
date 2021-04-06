#version 330 core

uniform sampler2D uDiffuse;
uniform vec3 color;
out vec4 fragColor;
in vec2 uv;

void main()
{
    fragColor = texture(uDiffuse, uv.xy);
    //fragColor = vec4(n, 1.0);
}
