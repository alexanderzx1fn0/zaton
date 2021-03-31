#version 330 core

uniform vec3 Color;
in vec3 n;
in vec2 uv;

layout( location = 0 ) out vec4 FragColor;
uniform int Pass;  // Pass number

uniform sampler2D HdrTex;
uniform sampler2D BlurTex1;
uniform sampler2D BlurTex2;

uniform float LumThresh;  // Luminance threshold

uniform float PixOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];

uniform mat3 rgb2xyz = mat3( 
  0.4124564, 0.2126729, 0.0193339,
  0.3575761, 0.7151522, 0.1191920,
  0.1804375, 0.0721750, 0.9503041 );

uniform mat3 xyz2rgb = mat3(
  3.2404542, -0.9692660, 0.0556434,
  -1.5371385, 1.8760108, -0.2040259,
  -0.4985314, 0.0415560, 1.0572252 );

uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform float AveLum;

/*
float luminance( vec3 color ) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}
*/

float luminance(in vec3 rgb) {
	const vec3 kLum = vec3(0.2126, 0.7152, 0.0722);
	return max(dot(rgb, kLum), 0.0001); // prevent zero result
}

// The render pass
vec4 pass1()
{
  return vec4(Color, 1.0f);
}

// Bright-pass filter (write to BlurTex1)
vec4 pass2() {
    vec4 val = texture(HdrTex, uv);
    if( luminance(val.rgb) > LumThresh )
        return val;
    else
        return vec4(0.0);
}
// debug
vec4 pass6() {
    return texture(HdrTex, uv) + texture(BlurTex1, uv);
}

// First blur pass (read from BlurTex1, write to BlurTex2)
vec4 pass3() {
    // get size of single texel
    float dy = 1.0 / (textureSize(BlurTex1,0)).y;

    vec4 sum = texture(BlurTex1, uv) * Weight[0];
    for( int i = 1; i < 10; i++ )
    {
        // up towards
         sum += texture( BlurTex1, uv + vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
         // down
         sum += texture( BlurTex1, uv - vec2(0.0,PixOffset[i]) * dy ) * Weight[i];
    }
    return sum;
}

// Second blur (read from BlurTex2, write to BlurTex1) 
vec4 pass4() {
    // get size of single texel
    float dx = 1.0 / (textureSize(BlurTex2,0)).x;

    vec4 sum = texture(BlurTex2, uv) * Weight[0];
    for( int i = 1; i < 10; i++ )
    {
        // go to the right towards of texel
       sum += texture( BlurTex2, uv + vec2(PixOffset[i],0.0) * dx ) * Weight[i];
         // go the lefth towards of texe
       sum += texture( BlurTex2, uv - vec2(PixOffset[i],0.0) * dx ) * Weight[i];
    }
    return sum;
}

// Composite pass, apply tone map to HDR image,
// then combine with the blurred bright-pass filter.
// (Read from BlurTex1 and HdrTex, write to default buffer).
vec4 pass5() {
    /////////////// Tone mapping ///////////////
    // Retrieve high-res color from texture
    vec4 color = texture( HdrTex, uv );
    
    // Convert to XYZ
    vec3 xyzCol = rgb2xyz * vec3(color);

    // Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    // Apply the tone mapping operation to the luminance (xyYCol.z or xyzCol.y)
    float L = (Exposure * xyYCol.z) / 1.0f;
    //float L = (Exposure * xyYCol.z) / AveLum;
    L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

    // Using the new luminance, convert back to XYZ
    xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
    xyzCol.y = L;
    xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

    // Convert back to RGB
    vec4 toneMapColor = vec4( xyz2rgb * xyzCol, 1.0);

    ///////////// Combine with blurred texture /////////////
    // We want linear filtering on this texture access so that
    // we get additional blurring.
    vec4 blurTex = texture(BlurTex1, uv);
    
   // ivec2 blurSize = textureSize(BlurTex1, 0);
   // if( gl_FragCoord.x < blurSize.x && gl_FragCoord.y < blurSize.y )
   //    return texture( BlurTex1, vec2(gl_FragCoord.x / blurSize.x,
   //    gl_FragCoord.y / blurSize.y ) );
  //  else
     return toneMapColor + blurTex;
}

void main() {
  if(Pass == 1) FragColor = pass1();
  else if(Pass == 2) FragColor = pass2();
  else if(Pass == 3) FragColor = pass3();
  else if(Pass == 4) FragColor = pass4();
  else if(Pass == 5) FragColor = pass5();
  else if(Pass == 6) FragColor = pass6();
}
