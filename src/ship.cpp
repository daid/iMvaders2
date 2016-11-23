#include "ship.h"
#include "spacescene.h"
#include "equipmentTemplate.h"
#include <sp2/graphics/spriteManager.h>
#include <sp2/logging.h>
#include <sp2/assert.h>

Ship::Ship()
: sp::SceneNode(space_scene->getRoot())
{
    reactor = EquipmentTemplate::create("REACTOR-1", this);
    engine = EquipmentTemplate::create("ENGINE-1", this);
    shield = EquipmentTemplate::create("SHIELD-1", this);
    hull = EquipmentTemplate::create("HULL-1", this);
    weapon[0] = EquipmentTemplate::create("BASIC-PULSE-LASER", this);
    weapon[1] = EquipmentTemplate::create("DOUBLE-PULSE-LASER", this);
    
    base_mass = 1.0;
}

Ship::~Ship()
{
    delete *controller;

    delete *reactor;
    delete *engine;
    delete *shield;
    delete *hull;
    delete *weapon[0];
    delete *weapon[1];
}

double Ship::getTotalMass()
{
    double mass = base_mass;
    if (reactor)
        mass += reactor->mass;
    if (engine)
        mass += engine->mass;
    if (shield)
        mass += shield->mass;
    if (hull)
        mass += hull->mass;
    if (weapon[0])
        mass += weapon[0]->mass;
    if (weapon[1])
        mass += weapon[1]->mass;
    return mass;
}

void Ship::takeDamage(double amount)
{
    if (shield)
        amount = shield->takeDamage(amount);
    if (hull->takeDamage(amount))
    {
        //We died...
        delete this;
    }
}

void Ship::onFixedUpdate()
{
    controller->update(this);
    
    if (weapon[0])
        weapon[0]->fire = controller->primary_fire;
    if (weapon[1])
        weapon[1]->fire = controller->secondary_fire;
}

bool Ship::changeReactor(sp::string id)
{
    sp::P<Equipment> e = EquipmentTemplate::create(id, this);
    if (!e)
        return false;
    delete *reactor;
    reactor = e;
    return true;
}

bool Ship::changeEngine(sp::string id)
{
    sp::P<Equipment> e = EquipmentTemplate::create(id, this);
    if (!e)
        return false;
    delete *engine;
    engine = e;
    return true;
}

bool Ship::changeShield(sp::string id)
{
    sp::P<Equipment> e = EquipmentTemplate::create(id, this);
    if (!e)
        return false;
    delete *shield;
    shield = e;
    return true;
}

bool Ship::changeHull(sp::string id)
{
    sp::P<Equipment> e = EquipmentTemplate::create(id, this);
    if (!e)
        return false;
    delete *hull;
    hull = e;
    return true;
}

bool Ship::changeWeapon(int index, sp::string id)
{
    sp2assert(index >= 0 && index < 2, "Weapon index out of range.");
    
    if (id == "")
    {
        delete *weapon[index];
        return true;
    }else{
        sp::P<Equipment> e = EquipmentTemplate::create(id, this);
        if (!e)
            return false;
        delete *weapon[index];
        weapon[index] = e;
        return true;
    }
}
