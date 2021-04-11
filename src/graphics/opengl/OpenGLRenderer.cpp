#include "OpenGLRenderer.h"

int OpenGLRenderer::nMeshes = 0;

OpenGLRenderer::OpenGLRenderer()
    : mesh(nullptr), Renderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
    delete currentShader;
    for (int i = 0; i < OpenGLRenderer::nMeshes; i++)
    {
	delete batch[i];
    }
    //delete mesh;
}

void OpenGLRenderer::drawIndexed(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices, const float* customNormals, const float* texcoord)
{
	batch[OpenGLRenderer::nMeshes++] = new Mesh(vertices, nVertices, indices, nIndices, customNormals, texcoord);
}

void OpenGLRenderer::drawIndexedTest(const simpleVertex* vertices, int nVertices, const unsigned int* indices, int nIndices)
{

    batch[OpenGLRenderer::nMeshes++] = new Mesh(vertices, nVertices, indices, nIndices);
}

void OpenGLRenderer::drawIndexedModel(const Vertex* vertices, int nVertices,
        const unsigned int* indices, int nIndices)
{
    batch[OpenGLRenderer::nMeshes++] = new Mesh(vertices, nVertices, indices, nIndices);

}



void OpenGLRenderer::setProjectionMatrix(const mat4* matProj)
{
    glUniformMatrix4fv(currentShader->getUniform("uProjM"), 1, GL_FALSE, (float*)matProj);
}

void OpenGLRenderer::setViewMatrix(const mat4* matView)
{
    glUniformMatrix4fv(currentShader->getUniform("uViewM"), 1, GL_FALSE, (float*)matView);
}
void OpenGLRenderer::setModelMatrix(const mat4* matModel)
{
    glUniformMatrix4fv(currentShader->getUniform("uModelM"), 1, GL_FALSE, (float*)matModel);
}

void OpenGLRenderer::setViewProjMatrix(const mat4* matViewProj)
{
    glUniformMatrix4fv(currentShader->getUniform("uViewProjM"), 1, GL_FALSE, (float*)matViewProj);

}

void OpenGLRenderer::setUniform1i(const char* str, int v)
{
    glUniform1i(currentShader->getUniform(str), v);

}

void OpenGLRenderer::addShader(const char* path1, const char *path2)
{
    currentShader = new Shader(path1, path2);
    currentShader->bind();
}

void OpenGLRenderer::changeShader(Shader* shader)
{
    currentShader = shader;
    currentShader->bind();
}
