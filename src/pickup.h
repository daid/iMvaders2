#ifndef PICKUP_H
#define PICKUP_H

#include <sp2/scene/node.h>

class Pickup : public sp::Node
{
public:
    Pickup(sp::Vector2d position);
    
    virtual void onCollision(sp::CollisionInfo& info) override;
};

#endif//PICKUP_H
