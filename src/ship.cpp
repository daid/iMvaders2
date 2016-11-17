#include "ship.h"
#include "spacescene.h"
#include "equipmentTemplate.h"
#include "weapon/bullet.h"
#include <sp2/graphics/spriteManager.h>
#include <sp2/logging.h>

Ship::Ship()
: sp::SceneNode(space_scene->getRoot())
{
    reactor = EquipmentTemplate::create("REACTOR-1", this);
    engine = EquipmentTemplate::create("ENGINE-1", this);
    shield = EquipmentTemplate::create("SHIELD-1", this);
    hull = EquipmentTemplate::create("HULL-1", this);
    weapon[0] = EquipmentTemplate::create("BASIC-PULSE-LASER", this);
    weapon[1] = EquipmentTemplate::create("DOUBLE-PULSE-LASER", this);
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

void Ship::takeDamage(double amount)
{
    amount = shield->takeDamage(amount);
    if (hull->takeDamage(amount))
    {
        //We died...
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
