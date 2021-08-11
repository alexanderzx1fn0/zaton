#include "SkyPlane.h"
#include "src/graphics/opengl/OpenGLExtensions.h"



SkyPlane::SkyPlane()
{
    vertices.resize(NUM_VERTICES);
    indices.resize(NUM_INDICES);

}

SkyPlane::~SkyPlane()
{}

bool SkyPlane::create()
{
    if (!createBuffers())
    {
        return false;
    }
    
    return true;
}

bool SkyPlane::createBuffers()
{
    
    for (int i = 0; i <= DIV_Y; i++)
    {
        for (int j = 0; j <= DIV_X; j++)
        {
            int index = i * 5 + j;
            float fX = 1.0f - (float)j / (float)(DIV_X);
            float fY = 1.0f - (float)i / (float)(DIV_Y);
            // map values from [1, 0] to range [-1, 1]
            glm::vec3 vPos = glm::vec3(fX*2.0f-1.0f, fY*2.0f-1.0f, 1.0f);
            vertices[index] = vPos;

        }
    }


    int baseIndex = 0;
    int numVertesPerRow = 5;

    for (int i = 0; i < DIV_Y; ++i)
    {
        for (int j = 0; j <= DIV_X; ++j)
        {
            indices[baseIndex++] = i * (DIV_X + 1) +j;
            indices[baseIndex++] = (i + 1) * (DIV_X + 1) +j;
            
        }
        if (i + 1 < DIV_Y)
        {
            indices[baseIndex++] = (i+1)*(DIV_X+1) + DIV_X;
            indices[baseIndex++] = (i+1)*(DIV_X+1);
        }
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vb);
    glBindBuffer(GL_ARRAY_BUFFER, _vb);

    glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0); // position
    glEnableVertexAttribArray( 0 );
    
    glGenBuffers(1, &_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return true;
    return true;
}

void SkyPlane::draw()
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTICES, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SkyPlane::remove()
{
}
