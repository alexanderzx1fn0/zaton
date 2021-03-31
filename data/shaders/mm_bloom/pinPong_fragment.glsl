#version 330 core

uniform vec3 Color;
in vec3 n;
in vec2 uv;

layout( location = 0 ) out vec4 FragColor;

uniform sampler2D BlurSampler;
uniform float	blurness;

void main(void)
{
    vec2 tx  = uv.xy;
    vec2 dx  = vec2 (0.019, 0.0);
    dx*=blurness;
    vec2 sdx = vec2(dx);
    vec4 sum = texture ( BlurSampler, tx ) * 0.134598;

    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.127325;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.107778;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.081638;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.055335;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.033562;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.018216;
    sdx += dx;
    sum += (texture ( BlurSampler, tx + sdx ) + texture ( BlurSampler, tx - sdx ) )* 0.008847;
    sdx += dx;

    FragColor = sum;
}
