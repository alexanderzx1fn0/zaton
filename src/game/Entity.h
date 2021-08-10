#ifndef ENTITY_H_
#define ENTITY_H_

#include "util/utils.h"
#include "graphics/Model.h"

class Entity
{
public:
    Entity();
    ~Entity();

    Model obj;

    Sphere s;
    vec3 pos;
    vec3 rot;

    mat4 mWorldTransform;
};


extern Entity* entity;
extern int entityCount;
typedef Entity* GEntity;
extern GEntity entities[];
#endif
