// 
// File: terrain.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a 3D terrain.       
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __terrainH__
#define __terrainH__


#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "src/graphics/Texture.h"

class Terrain
{
public:
	Terrain(
		std::string heightmapFileName, 
		int numVertsPerRow,  
		int numVertsPerCol, 
		int cellSpacing,    // space between cells
		float heightScale,
		float tu,float tv,bool stretch_area);   

	~Terrain();

	int  getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);

        void loadTexture(const char* filename);

	float getHeight(float x, float z);

	//bool loadTexture(WCHAR fileName[MAX_PATH]);
	//bool genTexture();
	//bool lightTerrain(D3DXVECTOR3* directionToLight);
	bool draw(glm::mat4* world);//, bool drawTris);
	bool hitInTerrain(float x,float z);
        Texture* texture;

private:
        unsigned int _tex;
        unsigned int _vao;
        unsigned int _vb;
        unsigned int _ib;

	int _numVertsPerRow;
	int _numVertsPerCol;
	int _cellSpacing;

	int _numCellsPerRow;
	int _numCellsPerCol;
	int _width;
	int _depth;
	int _numVertices;
	int _numTriangles;
        int _numIndices;

	float _heightScale;

	std::vector<int> _heightmap;


	// helper methods
	bool  readRawFile(std::string fileName);
	bool  computeVertices(float tu,float tv,bool stretch_area);
	bool  computeIndices();
        bool  createBuffer();
	float Terrain::Lerp(float a, float b, float t);
	//bool  lightTerrain(D3DXVECTOR3* directionToLight);
	//float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);

	struct TerrainVertex
	{
		TerrainVertex(){}
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

	};
	std::vector<TerrainVertex> vertices;
	std::vector<unsigned int> indices;

};

extern Terrain * TheTerrain;
extern Terrain * adjTerrainPointer;

#endif // __terrainH__
