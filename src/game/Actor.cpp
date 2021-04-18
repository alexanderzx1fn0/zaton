#include "Actor.h"

Actor::Actor(Game* game)
{
    game->addActor(this);
}
