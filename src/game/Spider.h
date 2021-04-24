#ifndef SPIDER_H_
#define SPIDER_H_

#include "Actor.h"

class Spider : public Actor
{
public:
    Spider(class Game* game);
    ~Spider();

    class MeshComponent* mMeshComponent;
};

#endif
