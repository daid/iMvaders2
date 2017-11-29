#ifndef ABANDONDED_PRINTER_H
#define ABANDONDED_PRINTER_H

#include "../spaceObject.h"

class AbandondedPrinter : public SpaceObject
{
public:
    AbandondedPrinter();
    
    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source, DamageType type) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onFixedUpdate();

private:
    float health;
};

#endif//ABANDONDED_PRINTER_H
