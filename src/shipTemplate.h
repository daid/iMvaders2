#ifndef SHIP_TEMPLATE_H
#define SHIP_TEMPLATE_H

#include "ship.h"

class ShipTemplate
{
public:
    static void init();
    
    static sp::P<Ship> create(sp::string id);

private:
    sp::string name;
    sp::Texture* texture;
    sp::Vector2f sprite_size;
    sp::Vector2d collision_size;
    
    sp::string reactor;
    sp::string engine;
    sp::string shield;
    sp::string hull;
    sp::string weapon[2];

    static std::map<sp::string, ShipTemplate> templates;
};

#endif//SHIP_TEMPLATE_H
