#ifndef SHIP_H
#define SHIP_H

#include <sp2/scene/node.h>
#include "shipcontroller.h"
#include "shipequipment/reactor.h"
#include "shipequipment/shield.h"
#include "shipequipment/hull.h"
#include "shipequipment/engine.h"
#include "weapon/weapon.h"

class Ship : public sp::SceneNode
{
public:
    Ship();
    ~Ship();
    
    sp::P<ShipController> controller;

    void takeDamage(double amount);
    double getTotalMass();

    virtual void onFixedUpdate() override;
    
    double base_mass;
    sp::P<Reactor> reactor;
    sp::P<Engine> engine;
    sp::P<Shield> shield;
    sp::P<Hull> hull;
    sp::P<Weapon> weapon[2];
    //special item
};

#endif//SHIP_H
