#ifndef ZTN_3DOBJ
#define ZTN_3DOBJ

#include "util/utils.h"
#include "graphics/data/Mesh.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

enum Type : int {
    MESH,
    LAMP,
    RESPAWN,
};

struct Model
{
    Model()
        : diffuseMap(NULL),
          normalMap(NULL),
          specularMap(NULL),
          vertices(NULL),
          indices(NULL),
          mesh(NULL),
          f_vertices(NULL)
    {
    }
    ~Model()
    {
        delete diffuseMap;
        delete normalMap;
        delete specularMap;
        delete [] vertices;
        delete [] indices;
        delete mesh;
        delete [] f_vertices;
    }
    Type type;
    Mesh* mesh;
    mat4 matrix;
    Texture* diffuseMap;
    Texture* normalMap;
    Texture* specularMap;
    int   nIndices;
    int   nVertices;
    unsigned int   *indices;
    Vertex  *vertices;
    simpleVertex* f_vertices;
    texturedVertex* t_vertices;
    int textured;
};

#endif
