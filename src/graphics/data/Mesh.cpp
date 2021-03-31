
#include "Mesh.h"

Mesh::Mesh(const float* vertices, int nVertices,
	const unsigned int* indices, int nIndices, const float* customNormals, const float* texcoord)
{
    index_count = nIndices;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
    glEnableVertexAttribArray( 0 );

    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 3, customNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // normals
    glEnableVertexAttribArray( 1 );

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 2, texcoord, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // texcoord
    glEnableVertexAttribArray( 2 );
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    float *ptr = 0;
    glBindVertexArray(0);


}

Mesh::~Mesh()
{
    /* делаем активным массив вершин */
    glBindVertexArray(vao);
    /* отцепляем буфер */
    // TODO: correct way for clear buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);
    /* делаем неактивным массив вершин */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);

}

void Mesh::draw_mesh()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
