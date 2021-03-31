#version 330 core

uniform sampler2D Sample0;

uniform int BlurAmount;
uniform float BlurScale;
uniform float BlurStrength;


in vec3 n;
in vec2 uv;

uniform float PixOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
layout( location = 0 ) out vec4 FragColor;

/// <summary>
/// Gets the Gaussian value in the first dimension.
/// </summary>
/// <param name="x">Distance from origin on the x-axis.</param>
/// <param name="deviation">Standard deviation.</param>
/// <returns>The gaussian value on the x-axis.</returns>
float Gaussian (float x, float deviation)
{
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
}

void main ()
{
	float dy = 1.0 / (textureSize(Sample0, 0)).y;
	// Locals
	float halfBlur = float(BlurAmount) * 0.5;
	vec4 colour = vec4(0.0);
	vec4 texColour = vec4(0.0);
	
	// Gaussian deviation
	float deviation = halfBlur * 0.35;
	deviation *= deviation;
	float strength = 1.0 - BlurStrength;
/*
	
	// Vertical blur
	for (int i = 0; i < 60; ++i)
	{
		if ( i >= BlurAmount )
			break;
		
		float offset = float(i) - halfBlur;
		texColour = texture(Sample0, uv + vec2(0.0, offset * dy * BlurScale)) * Gaussian(offset * strength, deviation);
		colour += texColour;
	}
*/
	for (int i = 0; i < 10; i++) {
	    if ( i >= BlurAmount )
		    break;
	    
	    float offset = float(i) - halfBlur;
	    colour += texture(Sample0, uv + vec2(PixOffset[i], 0.0) * dy * BlurScale ) * Gaussian(offset * strength, deviation); 
	    colour += texture(Sample0, uv - vec2(PixOffset[i], 0.0) * dy * BlurScale) * Gaussian(offset * strength, deviation); 
	}
	
	// Apply colour
	FragColor = clamp(colour, 0.0, 1.0);
	FragColor.w = 1.0;
	FragColor = colour;
}
