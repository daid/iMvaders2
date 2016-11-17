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
    sp::string sprite_name;
    sp::Vector2d collision_size;

    static std::map<sp::string, ShipTemplate> templates;
};

#endif//SHIP_TEMPLATE_H
