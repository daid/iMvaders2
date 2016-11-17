#ifndef SHIP_CONTROLLER_H
#define SHIP_CONTROLLER_H

#include <sp2/pointer.h>
#include <sp2/math/vector.h>

class Ship;
class ShipController : public sp::AutoPointerObject
{
public:
    ShipController();
    
    sp::Vector2d trust;
    float rotate;
    bool primary_fire;
    bool secondary_fire;
    bool brake;
    
    virtual void update(Ship* ship) = 0;
};

#endif//SHIP_CONTROLLER_H
