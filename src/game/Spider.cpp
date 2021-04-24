#include "Spider.h"
#include "core/Game.h"
#include "MeshComponent.h"

Spider::Spider(Game* game)
    : Actor(game)
{
    mMeshComponent = new MeshComponent(this);
}

Spider::~Spider()
{

}
