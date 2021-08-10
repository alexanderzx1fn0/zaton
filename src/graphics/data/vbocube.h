#ifndef VBO_CUBE_H_
#define VBO_CUBE_H_

#include "vertex.h"


inline buffers CreateBuffersCube(float scx,float scy,float scz,float tu,float tv,float tu1,float tv1)
{
	buffers obj;
	obj.lod_vb = 24;
	obj.lod_ib = 36;
	// save a ptr to the device

	// build box
	float xu = scx*tu;//Multi scx and tu
	float yv = scy*tv;
	float zu = scz*tu;

	float xu1 = scx*tu1;//Multi scx and tu
	float yv1 = scz*tv1;

//	scx = 1.0f;
//	scy = 1.0f;
//	scz = 1.0f;
        VertexPNT vertices[24];

	vertices[0] = VertexPNT(-scx, -scy, -scz, 0.0f, 0.0f, -1.0f, 0.0f, yv);
	vertices[1] = VertexPNT(-scx,  scy, -scz, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[2] = VertexPNT( scx,  scy, -scz, 0.0f, 0.0f, -1.0f, xu, 0.0f);
	vertices[3] = VertexPNT( scx, -scy, -scz, 0.0f, 0.0f, -1.0f, xu, yv);

	// fill in the back face verticesertex data
	vertices[4] = VertexPNT(-scx, -scy, scz, 0.0f, 0.0f, 1.0f, 0.0f, yv);
	vertices[5] = VertexPNT( scx, -scy, scz, 0.0f, 0.0f, 1.0f, xu, yv);
	vertices[6] = VertexPNT( scx,  scy, scz, 0.0f, 0.0f, 1.0f, xu, 0.0f);
	vertices[7] = VertexPNT(-scx,  scy, scz, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

	// fill in the top face verticesertex data
	vertices[8]  = VertexPNT(-scx, scy, -scz, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[9]  = VertexPNT(-scx, scy,  scz, 0.0f, 1.0f, 0.0f, 0.0f, yv1);
	vertices[10] = VertexPNT( scx, scy,  scz, 0.0f, 1.0f, 0.0f, xu1, yv1);
	vertices[11] = VertexPNT( scx, scy, -scz, 0.0f, 1.0f, 0.0f, xu1, 0.0f);

	// fill in the bottom face verticesertex data
	vertices[12] = VertexPNT(-scx, -scy, -scz, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[13] = VertexPNT( scx, -scy, -scz, 0.0f, -1.0f, 0.0f, xu1, 0.0f);
	vertices[14] = VertexPNT( scx, -scy,  scz, 0.0f, -1.0f, 0.0f, xu1, yv1);
	vertices[15] = VertexPNT(-scx, -scy,  scz, 0.0f, -1.0f, 0.0f, 0.0f, yv1);

	// fill in the left face verticesertex data
	vertices[16] = VertexPNT(-scx, -scy,  scz, -1.0f, 0.0f, 0.0f, 0.0f, yv);
	vertices[17] = VertexPNT(-scx,  scy,  scz, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[18] = VertexPNT(-scx,  scy, -scz, -1.0f, 0.0f, 0.0f, zu, 0.0f);
	vertices[19] = VertexPNT(-scx, -scy, -scz, -1.0f, 0.0f, 0.0f, zu, yv);

	// fill in the right face verticesertex data
	vertices[20] = VertexPNT( scx, -scy, -scz, 1.0f, 0.0f, 0.0f, 0.0f, yv);
	vertices[21] = VertexPNT( scx,  scy, -scz, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[22] = VertexPNT( scx,  scy,  scz, 1.0f, 0.0f, 0.0f, zu, 0.0f);
	vertices[23] = VertexPNT( scx, -scy,  scz, 1.0f, 0.0f, 0.0f, zu, yv);




	unsigned int indices[36];

	// findicesll indicesn the front face indicesndex data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// findicesll indicesn the back face indicesndex data
	indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// findicesll indicesn the top face indicesndex data
	indices[12] = 8; indices[13] =  9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// findicesll indicesn the bottom face indicesndex data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// findicesll indicesn the left face indicesndex data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;
	// findicesll indicesn the rindicesght face indicesndex data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;

	indices[33] = 20; indices[34] = 22; indices[35] = 23;

        glGenVertexArrays(1, &obj.vao);
        glBindVertexArray(obj.vao);

        glGenBuffers(1, &obj.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);

        glBufferData(GL_ARRAY_BUFFER, obj.lod_vb * sizeof(VertexPNT), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &obj.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.lod_ib * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        // set memory layout of vertices: px, py, pz, nx, ny, nz, tx, ty
                                        //0-----------12----------24---;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT) , (GLvoid*)0); // position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT),  (GLvoid*)12); // normal
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid*)24); // texcoords

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);


        glBindVertexArray(0);

	return obj;
}

#endif
