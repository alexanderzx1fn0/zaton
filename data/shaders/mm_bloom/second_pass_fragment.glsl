#version 330 core

uniform vec3 Color;
in vec3 n;
in vec2 uv;

layout( location = 0 ) out vec4 FragColor;
uniform sampler2D DownSampler;
uniform float LumThresh;  // Luminance threshold

float luminance(in vec3 rgb) {
	const vec3 kLum = vec3(0.2126, 0.7152, 0.0722);
	return max(dot(rgb, kLum), 0.0001); // prevent zero result
}

// Bright-pass filter (write to BlurTex1)
vec4 brightPass() {
    vec4 val = texture(DownSampler, uv);
    if( luminance(val.rgb) > LumThresh )
        return val;
    else
        return vec4(0.0);
}

void main() {
	FragColor = brightPass();
}

