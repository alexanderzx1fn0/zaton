#ifndef OPENGL_RENDERER_H_
#define OPENGL_RENDERER_H_

#include "graphics/Renderer.h"
#include "graphics/data/Mesh.h"

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    void drawIndexed(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices, const float* customNormals, const float* texcoord);

    void drawIndexedTest(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices);

    void setProjectionMatrix(const mat4* matProj);
    void setViewMatrix(const mat4* matView);
    void setModelMatrix(const mat4* matModel);
    void setViewProjMatrix(const mat4* matViewProj);
    
    void addShader(const char* path1, const char *path2);
    void changeShader(Shader* shader);

    Mesh* mesh;
    Mesh* batch[6];// TODO: actually it should be arbitrary size array
    static int nMeshes;
};



#endif // OPENGL_RENDERER_H_
