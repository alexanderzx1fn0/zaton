#ifndef ENTITY_H_
#define ENTITY_H_

#include "util/utils.h"
#include "graphics/Model.h"

class Entity
{
public:
    Entity();

    Model* _obj;

    Sphere s;
    vec3 pos;
    vec3 rot;
};

extern Entity* entities[32];
// it's will member of Level class
extern entityCount = 0;

#endif
