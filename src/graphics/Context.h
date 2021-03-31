#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "utils.h"


typedef uint32 Index;

struct newVertex {
    vec3 coord;
    vec3 normal;
    vec2 texCoord; // ex vec4
    vec3 tangent;
    vec3 bitangent;
};
/*
struct Vertex {
    vec3 coord;
    vec3 normal;
    vec4 texCoord;
};
*/
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 texcoord;
};
namespace Context {
    int width, height;

}

#endif // CONTEXT_H_
