#ifndef ENTITY_H_
#define ENTITY_H_

#include "util/utils.h"

class Entity
{
public:
    Entity();

private:
    vec3 pos;
    vec3 rot;
    OBJ * obj; // MESH data
};

#endif
