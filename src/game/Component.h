#ifndef COMPONENT_H_
#define COMPONENT_H_

class Component
{
public:
    Component(class Actor* owner, int updateOrder = 100);
    ~Component();

    virtual void Update(float deltaTime);
    
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    class Actor* mOwner;
    int mUpdateOrder;
};

#endif
