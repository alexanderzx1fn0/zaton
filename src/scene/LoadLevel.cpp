#include "LoadLevel.h"
#include "graphics/data/Mesh.h"
#include "util/Stream.h"
#include "game/Entity.h"
#include "graphics/Model.h"

bool LoadCollidableGeometry()
{
    Stream stream("../data/environment.lvl");
    while (stream.pos < stream.size)
    {
        entity =(entities[entityCount++] = new Entity());

        stream.read(&entity->obj.type, sizeof(entity->obj.type));
        stream.read(&entity->obj.matrix, sizeof(entity->obj.matrix));

        if (entity->obj.type != MESH) continue;

        stream.read(&entity->obj.nIndices, sizeof(entity->obj.nIndices));
        entity->obj.indices = new unsigned[entity->obj.nIndices];
        stream.read(entity->obj.indices, entity->obj.nIndices * sizeof(unsigned int));

        stream.read(&entity->obj.nVertices, sizeof(entity->obj.nVertices));
        entity->obj.f_vertices = new float[entity->obj.nVertices * 3]; // Each vertex has three floats (x, y, z)
        stream.read(entity->obj.f_vertices, entity->obj.nVertices * sizeof(float) * 3);
    }
    return true;
}
void ClearCollidableGeometry()
{

}



bool LoadLevel()
{
    Stream stream("../data/barrel.geom");
    while (stream.pos < stream.size)
    {
        entity =(entities[entityCount++] = new Entity());

        stream.read(&entity->obj.type, sizeof(entity->obj.type));
        stream.read(&entity->obj.matrix, sizeof(entity->obj.matrix));

        if (entity->obj.type != MESH) continue;

        char* tex1 = stream.readStr();
        char* tex2 = stream.readStr();
        char* tex3 = stream.readStr();
        if  (tex1) entity->obj.diffuseMap = new Texture(tex1);
        if  (tex2) entity->obj.normalMap = new Texture(tex2);
        if  (tex3) entity->obj.specularMap = new Texture(tex3);


        delete[] tex1;
        delete[] tex2;
        delete[] tex3;

        stream.read(&entity->obj.nIndices, sizeof(entity->obj.nIndices));
        entity->obj.indices = new unsigned[entity->obj.nIndices];
        stream.read(entity->obj.indices, entity->obj.nIndices * sizeof(unsigned int));

        stream.read(&entity->obj.nVertices, sizeof(entity->obj.nVertices));
        entity->obj.vertices = new Vertex[entity->obj.nVertices]; // Each vertex has three floats (x, y, z)
        stream.read(entity->obj.vertices, entity->obj.nVertices * sizeof(Vertex));
    }
    return true;
}

void ClearLevel()
{
    for (int i = 0; i < entityCount; i++)
    {
        delete entities[i];
    }
    //delete entity;
}
