#include "DrawDebug.h"
#include "Renderer.h"

void Line::create()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2* sizeof(float) * 3, 0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
    glEnableVertexAttribArray( 0 );

    glBindVertexArray(0);
}

void Line::draw(const vec3& ro, const vec3& rd, float t)
{
    printf("Draw line\n");
/*
    vertices[0] = ro.x;
    vertices[1] = ro.y;
    vertices[2] = ro.z;
    vertices[3] = ro.x;
    vertices[4] = ro.y;
    vertices[5] = t;
*/
    
    vertices[0] = ro.x;
    vertices[1] = ro.y;
    vertices[2] = ro.z;
    vertices[3] = rd.x * t;
    vertices[4] = rd.y * t;
    vertices[5] = rd.z *t;

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(float) * 3, vertices);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);

}

