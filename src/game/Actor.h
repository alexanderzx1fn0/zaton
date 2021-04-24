#ifndef ACTOR_H_
#define ACTOR_H_

#include <vector>

class Actor
{
public:
    Actor(class Game* game);
    virtual ~Actor();

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:
    std::vector<class Component*> mComponents;
    class Game* mGame;
};
#endif
