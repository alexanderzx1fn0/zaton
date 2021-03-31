#version 330 core

uniform vec3 Color;
in vec3 n;
in vec3 p;
in vec2 uv;

struct LightInfo
{
    vec3 Position;
    vec3 Intensity;
    vec3 LightColor;
};

uniform LightInfo lights[5];

uniform float Shininess; // Показатель степени отражения

uniform vec3 eyePosition; // is camera pos

vec3 ads(int lightIndex, vec3 position, vec3 norm)
{

    vec3 ambientLight = .1f * lights[0].LightColor;
    vec3 toLight = normalize(lights[0].Position - position);
    vec3 normal = normalize(norm);
    vec3 diffuseLight = max(dot(toLight, normal), 0.0) * lights[0].LightColor;

    float specularStrength = 0.5f;
    vec3 toView = normalize(eyePosition - position);
    vec3 toReflection  = reflect(-toView, normal);
    float specularAmount = pow(max(dot(toReflection, toLight), 0.0), Shininess);
    vec3 specularLight = specularStrength * lights[0].LightColor * specularAmount;

    return (ambientLight + diffuseLight + specularLight) * Color;
}



layout( location = 0 ) out vec4 FragColor;

void main() {
    FragColor = vec4(ads(0, p, n), 1.0);
}
