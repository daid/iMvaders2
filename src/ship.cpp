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
    hull = EquipmentTemplate::create("HULL-1", this);
    
    base_mass = 1.0;

    shield_damage_indicator = 0;
    hull_damage_indicator = 0;
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

    if (amount == 0)
        shield_damage_indicator = 25;
    else
        hull_damage_indicator = 25;
    
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
    
    if (shield_damage_indicator > 0)
        shield_damage_indicator--;
    if (hull_damage_indicator > 0)
        hull_damage_indicator--;
    
    if ((hull_damage_indicator % 2) == 1)
    {
        render_data.color = sf::Color(255, 255, 255, 128);
    }
    else if ((shield_damage_indicator % 4) >= 2)
    {
        render_data.color = sf::Color(255, 255, 255, 192);
    }
    else
    {
        render_data.color = sf::Color::White;
    }
    sp::Vector2d position = getGlobalPosition2D();
    if (position.x > 15)
        setLinearVelocity(getLinearVelocity2D() + sp::Vector2d(-(position.x - 15), 0));
    if (position.y > 20)
        setLinearVelocity(getLinearVelocity2D() + sp::Vector2d(0, -(position.y - 20)));
    if (position.x < -15)
        setLinearVelocity(getLinearVelocity2D() + sp::Vector2d(-(position.x + 15), 0));
    if (position.y < -20)
        setLinearVelocity(getLinearVelocity2D() + sp::Vector2d(0, -(position.y + 20)));
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
    if (id == "")
    {
        delete *shield;
        return true;
    }
    else
    {
        sp::P<Equipment> e = EquipmentTemplate::create(id, this);
        if (!e)
            return false;
        delete *shield;
        shield = e;
        return true;
    }
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
