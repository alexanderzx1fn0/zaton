#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/vec3.hpp>
#include "src/graphics/opengl/OpenGLExtensions.h"

struct VertexPNT
{
    VertexPNT(){}
    VertexPNT(
            float x, float y, float z,
            float nx, float ny, float nz,
            float u, float v)
    {
            pos.x  = x;  pos.y  = y;  pos.z  = z;
            n.x = nx; n.y = ny; n.z = nz;
            tu  = u;  tv  = v;
    }
    glm::vec3 pos;
    glm::vec3 n;
    float tu, tv;
};

struct buffers
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int lod_vb;
    int lod_ib;
};

#endif
