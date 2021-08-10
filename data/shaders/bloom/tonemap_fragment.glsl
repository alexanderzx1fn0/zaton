#version 330 core

in vec3 n;
in vec2 uv;

uniform int Pass;   // Pass number
uniform sampler2D renderTex;
uniform vec3 color;
uniform float Weight[5];

uniform vec2 u_viewPort;
uniform float AveLum; // average luminance

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HdrColor;

// XYZ/RGB conversion matrices from:
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

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
uniform bool DoToneMap = true;

void pass1() {
    //return vec4(blinnPhong( Position, normalize(Normal) ),1.0);
    //return vec4(color, 1.0f);
    HdrColor = vec3(color); 
}

// This pass computes the sum of the luminance of all pixels
void pass2() {
    // Retrieve high-res color from texture
    vec4 Color = texture( renderTex, uv );
    
    // Convert to XYZ
    vec3 xyzCol = rgb2xyz * vec3(Color);

    // Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    // Apply the tone mapping operation to the luminance (xyYCol.z or xyzCol.y)
    float L = (Exposure * xyYCol.z) / AveLum;
    L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

    // Using the new luminance, convert back to XYZ
    xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
    xyzCol.y = L;
    xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

    // Convert back to RGB and send to output buffer
    if( DoToneMap )
      FragColor = vec4( xyz2rgb * xyzCol, 1.0);
    else
      FragColor = Color;
}

void main() {
    if( Pass == 1 ) pass1();
    else if( Pass == 2 ) pass2();
}
