
#version 330 core
in vec2 uv;
uniform sampler2D Sampler0;

layout( location = 0 ) out vec4 FragColor;


uniform float PixOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];
uniform bool horizontal;

vec4 blur()
{

    vec4 sum = texture(Sampler0, uv) * Weight[0];
    float dx = 1.0 / (textureSize(Sampler0,0)).x;
    float dy = 1.0 / (textureSize(Sampler0,0)).y;

    if (horizontal) {
	for( int i = 1; i < 10; i++ )
	{
	    // go to the right towards of texel
	   sum += texture( Sampler0, uv + vec2(PixOffset[i],0.0) * dx ) * Weight[i];
	     // go the lefth towards of texe
	   sum += texture( Sampler0, uv - vec2(PixOffset[i],0.0) * dx ) * Weight[i];
	}

    }
    else {
	for( int i = 1; i < 10; i++ )
	{
	    // up towards
	     sum += texture( Sampler0, uv + vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
	     // down
	     sum += texture( Sampler0, uv - vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
	}

    }

    return sum;
}

void main()
{
    FragColor = blur();
}

/*
#version 330 core
in vec2 uv;
uniform sampler2D Sampler0;

layout( location = 0 ) out vec4 FragColor;

#define MAX_BLUR_RADIUS 4096
uniform int uBlurRadius = 16;
uniform float uBlurSigma = 2.0f;
uniform vec2 uBlurDirection = vec2(1.0, 0.0); // (1,0)/(0,1) for x/y pass
uniform bool horizontal;


//	Incremental, forward-differencing Gaussian elimination based on:
//	http://http.developer.nvidia.com/GPUGems3/gpugems3_ch40.html
vec4 incrementalGauss1D(
	in sampler2D srcTex, 
	in vec2 srcTexelSize, 
	in vec2 origin,
	in int radius,
	in vec2 direction
) {

	int nSamples = clamp(radius, 1, int(MAX_BLUR_RADIUS)) / 2;
	
	if (nSamples == 0)
		return texture(srcTex, origin);
	
	float SIGMA = float(uBlurRadius) / 8.0;
	float sig2 = SIGMA * SIGMA;
	const float TWO_PI	= 6.2831853071795;
	const float E			= 2.7182818284590;
		
//	set up incremental counter:
	vec3 gaussInc;
	gaussInc.x = 1.0 / (sqrt(TWO_PI) * SIGMA);
	gaussInc.y = exp(-0.5 / sig2);
	gaussInc.z = gaussInc.y * gaussInc.y;
	
//	accumulate results:
	vec4 result = texture(srcTex, origin) * gaussInc.x;	
	for (int i = 1; i < nSamples; ++i) {
		gaussInc.xy *= gaussInc.yz;
		
		vec2 offset = float(i) * direction * srcTexelSize;
		result += texture(srcTex, origin - offset) * gaussInc.x;
		result += texture(srcTex, origin + offset) * gaussInc.x;
	}
	
	return result;
}

void main() {
	vec2 texelSize = 1.0 / vec2(textureSize(Sampler0, 0));
	if (horizontal) {
	    FragColor = incrementalGauss1D(Sampler0, texelSize, uv, uBlurRadius,  vec2(1.0, 0.0)); 
	}
	else{
	    FragColor = incrementalGauss1D(Sampler0, texelSize, uv, uBlurRadius, vec2(0.0, 1.0)); 

	}
}
*/
