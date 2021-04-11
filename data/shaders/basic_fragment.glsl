#version 330 core

uniform sampler2D uDiffuse;
uniform vec3 color;
out vec4 fragColor;
in vec2 uv;
in vec3 n;


uniform bool colorLine;


void main()
{
    if (colorLine) {
	    fragColor = vec4(vec3(1.0f, 0.0, 0.0), 1.0f);
    }
    else {
	    fragColor = vec4(vec3(1.0), 1.0);
    }
    //fragColor = vec4(normalize(n), 1.0);
}
