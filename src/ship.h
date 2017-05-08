#ifndef SHIP_H
#define SHIP_H

#include "spaceObject.h"
#include "shipcontroller.h"
#include "shipequipment/reactor.h"
#include "shipequipment/shield.h"
#include "shipequipment/hull.h"
#include "shipequipment/engine.h"
#include "weapon/weapon.h"
#include "faction.h"

class Ship : public SpaceObject
{
public:
    Ship();
    ~Ship();
    
    sp::P<ShipController> controller;

    virtual bool takeDamage(double amount, DamageSource damage_source) override;
    double getTotalMass();

    virtual void onFixedUpdate() override;
    
    double base_mass;
    sp::P<Reactor> reactor;
    sp::P<Engine> engine;
    sp::P<Shield> shield;
    sp::P<Hull> hull;
    sp::P<Weapon> weapon[2];
    //special item
    
    sp::P<Faction> faction;
    
    bool changeReactor(sp::string id);
    bool changeEngine(sp::string id);
    bool changeShield(sp::string id);
    bool changeHull(sp::string id);
    bool changeWeapon(int index, sp::string id);
    
    int shield_damage_indicator;
    int hull_damage_indicator;
};

#endif//SHIP_H
