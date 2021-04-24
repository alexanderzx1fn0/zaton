#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_

#include "Component.h"


class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* owner);
    ~MeshComponent();

    class Mesh* mMesh;
};

#endif
