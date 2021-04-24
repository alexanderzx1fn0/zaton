#include "MeshComponent.h"
#include "Actor.h"

#include "graphics/data/Mesh.h"

MeshComponent::MeshComponent(Actor* actor)
    : Component(actor)
    , mMesh(nullptr)
{

}

MeshComponent::~MeshComponent()
{
    
}
