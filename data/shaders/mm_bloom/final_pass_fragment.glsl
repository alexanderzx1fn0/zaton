#version 330 core

uniform vec3 Color;
in vec3 n;
in vec2 uv;

layout( location = 0 ) out vec4 FragColor;

uniform sampler2D FullSampler;
uniform sampler2D BlurSampler;

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
uniform float AveLum = 1.0f;

void main() {
    /////////////// Tone mapping ///////////////
    // Retrieve high-res color from texture
    vec4 color = texture( FullSampler, uv );
    
    // Convert to XYZ
    vec3 xyzCol = rgb2xyz * vec3(color);

    // Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    // Apply the tone mapping operation to the luminance (xyYCol.z or xyzCol.y)
    //float L = (Exposure * xyYCol.z) / 1.0f;
    float L = (Exposure * xyYCol.z) / AveLum;
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
    vec4 blurTex = texture(BlurSampler, uv);
    
   // ivec2 blurSize = textureSize(BlurTex1, 0);
   // if( gl_FragCoord.x < blurSize.x && gl_FragCoord.y < blurSize.y )
   //    return texture( BlurTex1, vec2(gl_FragCoord.x / blurSize.x,
   //    gl_FragCoord.y / blurSize.y ) );
  //  else
    FragColor = toneMapColor + blurTex;
    //FragColor = vec4(vec3(AveLum), 1.0f);
}
