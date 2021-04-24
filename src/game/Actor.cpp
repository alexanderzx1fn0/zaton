#include "Actor.h"
#include "core/Game.h"
#include "Component.h"

Actor::Actor(Game* game)
    : mGame(game)
{
    game->AddActor(this);
}

Actor::~Actor()
{
    mGame->RemoveActor(this);
    // Because ~Component calls RemoveComponent, need a different style loop
    while (!mComponents.empty())
    {
            delete mComponents.back();
    }
}

void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
