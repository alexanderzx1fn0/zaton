#ifndef MESH_H_
#define MESH_H_

#include <string.h>
#include "graphics/opengl/OpenGLExtensions.h"

class Mesh
{
public:
    Mesh(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices, const float* customNormals, const float* texcoord);
    Mesh(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices);

    ~Mesh();

    void draw_mesh();

    GLuint vao, vbo, nbo, tbo, ebo;
    unsigned int index_count;
    unsigned int* pIndices;
};

#endif // MESH_H_
