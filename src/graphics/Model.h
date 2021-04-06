#ifndef ZTN_3DOBJ
#define ZTN_3DOBJ

#include "util/utils.h"
#include "graphics/data/Mesh.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

struct Model
{
    Mesh* mesh;
    mat4 matrix;
    Texture* diffuseMap;
    Texture* normalMap;
    Texture* specualMap;
    int   nIndices;
    int   nVertices;
    unsigned int   *indices;
    Vertex  *vertices;

};

#endif
