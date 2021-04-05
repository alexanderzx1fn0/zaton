#ifndef RENDERER_H_
#define RENDERER_H_

#include "Shader.h"
#include "util/utils.h"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void drawIndexed(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices, const float* customNormals, const float* texcoord) = 0;

    virtual void drawIndexedTest(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices) = 0;

    virtual void setProjectionMatrix(const mat4* matProj) = 0;
    virtual void setViewMatrix(const mat4* matView) = 0;
    virtual void setModelMatrix(const mat4* matModel) = 0;
    virtual void setViewProjMatrix(const mat4* matViewProj) = 0;

    // TODO: how to create shader you can see in ##ingredients
    virtual void addShader(const char* path1, const char *path2) = 0;
    virtual void changeShader(Shader* shader) = 0;


    // TODO: actually need list of shaders
    Shader* currentShader;
}; 
extern Renderer* GRenderer;

#endif
