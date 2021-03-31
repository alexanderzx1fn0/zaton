#version 330 core

in vec3 n;
in vec2 uv;

uniform int Pass;   // Pass number
uniform sampler2D renderTex;
uniform vec3 color;
uniform float Weight[5];

uniform vec2 u_viewPort;

layout (location = 0) out vec4 FragColor;

vec4 pass1() {
    //return vec4(blinnPhong( Position, normalize(Normal) ),1.0);
    return vec4(color, 1.0f);
}

vec4 pass2() {
    ivec2 pix = ivec2( gl_FragCoord.xy );
    vec4 sum = texelFetch(renderTex, pix, 0) * Weight[0];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,1) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-1) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,2) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-2) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,3) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-3) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,4) ) * Weight[4];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-4) ) * Weight[4];
    return sum;
}

vec4 pass3() {
    ivec2 pix = ivec2( gl_FragCoord.xy );
    vec4 sum = texelFetch(renderTex, pix, 0) * Weight[0];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(1,0) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-1,0) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(2,0) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-2,0) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(3,0) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-3,0) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(4,0) ) * Weight[4];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-4,0) ) * Weight[4];
    return sum;
 
}

void main() {
    if( Pass == 1 ) FragColor = pass1();
    else if( Pass == 2 ) FragColor = pass2();
    else if( Pass == 3 ) FragColor = pass3();
}
