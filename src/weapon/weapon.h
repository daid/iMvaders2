#ifndef WEAPON_H
#define WEAPON_H

#include "../shipequipment/equipment.h"

class Weapon : public Equipment
{
public:
    Weapon(sp::P<Ship> parent);
    
    bool fire;
    
    virtual bool hasHitPotential(sp::P<Ship> target) = 0;
private:
};

#endif//WEAPON_H
