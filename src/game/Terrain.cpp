//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: terrain.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a 3D terrain. 
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "Terrain.h"
#include <fstream>
#include <cmath>
#include "util/utils.h"
#include "src/graphics/opengl/OpenGLExtensions.h"

Terrain* TheTerrain = NULL;
Terrain * adjTerrainPointer = NULL;

Terrain::Terrain(
				 std::string heightmapFileName,
				 int numVertsPerRow,
				 int numVertsPerCol,
				 int cellSpacing,
				 float heightScale,
				 float tu,float tv,bool stretch_area)
{
        texture = NULL;
	_numVertsPerRow = numVertsPerRow;
	_numVertsPerCol = numVertsPerCol;
	_cellSpacing    = cellSpacing;

	_numCellsPerRow = _numVertsPerRow - 1;
	_numCellsPerCol = _numVertsPerCol - 1;

	_width = _numCellsPerRow * _cellSpacing;
	_depth = _numCellsPerCol * _cellSpacing;

	_numVertices  = _numVertsPerRow * _numVertsPerCol;
	_numTriangles = _numCellsPerRow * _numCellsPerCol * 2;

	_heightScale = heightScale;
	_numIndices = numVertsPerRow * numVertsPerCol * 6;
        vertices.resize(_numVertices);
        indices.resize(_numIndices);

	// load heightmap
	if( !readRawFile(heightmapFileName) )
	{
		//::MessageBox(0, L"readRawFile - FAILED", 0, 0);
	//	::PostQuitMessage(0);
	}

	// scale heights
	for(int i = 0; i < (int)_heightmap.size(); i++)
		_heightmap[i] = int(_heightmap[i]*heightScale);

	// compute the vertices
	if( !computeVertices(tu,tv,stretch_area) )
	{
		//::MessageBox(0, L"computeVertices - FAILED", 0, 0);
	//	::PostQuitMessage(0);
	}

	// compute the indices
	if( !computeIndices() )
	{
		//::MessageBox(0, L"computeIndices - FAILED", 0, 0);
	//	::PostQuitMessage(0);
	}
        if(!createBuffer())
        {

        }
}

Terrain::~Terrain()
{
        delete texture;
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &_vb);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &_ib);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &_vao);
}

void Terrain::loadTexture(const char* filename)
{
   texture = new Texture(filename); 
}

int Terrain::getHeightmapEntry(int row, int col)
{
	return _heightmap[row * _numVertsPerRow + col];
}

void Terrain::setHeightmapEntry(int row, int col, int value)
{
	_heightmap[row * _numVertsPerRow + col] = value;
}

bool Terrain::computeVertices(float tu,float tv,bool stretch_area)
{
	// coordinates to start generating vertices at
	int startX = -_width / 2;
	int startZ =  _depth / 2;

	// coordinates to end generating vertices at
	int endX =  _width / 2;
	int endZ = -_depth / 2;

	// compute the increment size of the texture coordinates
	// from one vertex to the next.
	float uCoordIncrementSize = tu;// / (float)_numCellsPerRow;
	float vCoordIncrementSize = tv;// / (float)_numCellsPerCol;

	if (stretch_area)
	{
		uCoordIncrementSize /= (float)_numCellsPerRow;
		vCoordIncrementSize /= (float)_numCellsPerCol;
	}


	int i = 0;
	for(int z = startZ; z >= endZ; z -= _cellSpacing)
	{
		int j = 0;

		for(int x = startX; x <= endX; x += _cellSpacing)
		{
			// compute the correct index into the vertex buffer and heightmap
			// based on where we are in the nested loop.

			int index = i * _numVertsPerRow + j;

			vertices[index] = TerrainVertex(
				(float)x,
				(float)_heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);

			j++; // next column
		}
		i++; // next row
	}

	return true;
}

bool Terrain::computeIndices()
{
	// index to start of a group of 6 indices that describe the
	// two triangles that make up a quad
	int baseIndex = 0;

	// loop through and compute the triangles of each quad
	for(int i = 0; i < _numCellsPerCol; i++)
	{
		for(int j = 0; j < _numCellsPerRow; j++)
		{

			indices[baseIndex]     = unsigned(  i   * _numVertsPerRow + j);
			indices[baseIndex + 1] = unsigned(  i   * _numVertsPerRow + j + 1);
			indices[baseIndex + 2] = unsigned((i+1) * _numVertsPerRow + j);

			indices[baseIndex + 3] = unsigned((i+1) * _numVertsPerRow + j);
			indices[baseIndex + 4] = unsigned(  i   * _numVertsPerRow + j + 1);
			indices[baseIndex + 5] = unsigned((i+1) * _numVertsPerRow + j + 1);

			// next quad
			baseIndex += 6;
		}
	}

	return true;
}

bool Terrain::createBuffer()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vb);
    glBindBuffer(GL_ARRAY_BUFFER, _vb);

    glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(TerrainVertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0); // position
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)12); // texcoords
    glEnableVertexAttribArray( 2 );
    
    glGenBuffers(1, &_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return true;
}




bool Terrain::readRawFile(std::string fileName)
{
	// Restriction: RAW file dimensions must be >= to the
	// dimensions of the terrain.  That is a 128x128 RAW file
	// can only be used with a terrain constructed with at most
	// 128x128 vertices.

	// A height for each vertex
	std::vector<unsigned char> in( _numVertices );
//	std::vector<INT> in( _numVertices );

	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);

	if( inFile.fail() )
		return false;

	inFile.read(
		(char*)&in[0], // buffer
		in.size());// number of bytes to read into buffer

	inFile.close();

	// copy BYTE vector to int vector
	_heightmap.resize( _numVertices );

	for(int i = 0; i < (int)in.size(); i++)
		_heightmap[i] = in[i];

	return true;
}

float Terrain::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

float Terrain::getHeight(float x, float z)
{
	if(!hitInTerrain(x,z))	return 0.0f;

	// Translate on xz-plane by the transformation that takes
	// the terrain START point to the origin.
	x = ((float)_width / 2.0f) + x;
	z = ((float)_depth / 2.0f) - z;

	// Scale down by the transformation that makes the 
	// cellspacing equal to one.  This is given by 
	// 1 / cellspacing since; cellspacing * 1 / cellspacing = 1.
	x /= (float)_cellSpacing;
	z /= (float)_cellSpacing;

	// From now on, we will interpret our positive z-axis as
	// going in the 'down' direction, rather than the 'up' direction.
	// This allows to extract the row and column simply by 'flooring'
	// x and z:

	float col = ::floorf(x);
	float row = ::floorf(z);

	int icol = int(col);
	int irow = int(row);

	// get the heights of the quad we're in:
	// 
    //  A   B
    //  *---*
    //  | / |
    //  *---*  
    //  C   D

	float A = (float)getHeightmapEntry(irow,   icol);
	float B = (float)getHeightmapEntry(irow,   icol+1);
	float C = (float)getHeightmapEntry(irow+1, icol);
	float D = (float)getHeightmapEntry(irow+1, icol+1);

	//
	// Find the triangle we are in:
	//

	// Translate by the transformation that takes the upper-left
	// corner of the cell we are in to the origin.  Recall that our 
	// cellspacing was nomalized to 1.  Thus we have a unit square
	// at the origin of our +x -> 'right' and +z -> 'down' system.
	float dx = x - col;
	float dz = z - row;

	// Note the below compuations of u and v are unneccessary, we really
	// only need the height, but we compute the entire vector to emphasis
	// the books discussion.
  float height = 0.0f;
	if(dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

		// Linearly interpolate on each vector.  The height is the vertex
		// height the vectors u and v originate from {A}, plus the heights
		// found by interpolating on each vector u and v.
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B

		// Linearly interpolate on each vector.  The height is the vertex
		// height the vectors u and v originate from {D}, plus the heights
		// found by interpolating on each vector u and v.
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}

bool Terrain::draw(glm::mat4* world)//, bool drawTris)
{
/*
	HRESULT hr = 0;

	if( _device )
	{
		_device->SetTransform(D3DTS_WORLD, world);

		_device->SetStreamSource(0, _vb, 0, sizeof(TerrainVertex));
		_device->SetFVF(TerrainVertex::FVF);
		_device->SetIndices(_ib);
		
		_device->SetTexture(0, _tex);

		// turn off lighting since we're lighting it ourselves
//		_device->SetRenderState(D3DRS_LIGHTING, false);

		hr =_device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			_numVertices,
			0,
			_numTriangles);

//		_device->SetRenderState(D3DRS_LIGHTING, true);
*/
/*
		if( drawTris )
		{
			_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr =_device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				_numVertices,
				0,
				_numTriangles);

			_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
*/
/*
		if(FAILED(hr))
			return false;
	}
*/
        
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    return true;
}

bool Terrain::hitInTerrain(float x,float z)
{
	if(fabs(x)<=_width/2-1.0f && fabs(z)<=_depth/2-1.0f)
		return true;
	return false;
}


