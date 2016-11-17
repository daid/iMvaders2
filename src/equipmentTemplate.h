#ifndef EQUIPMENT_TEMPLATE_H
#define EQUIPMENT_TEMPLATE_H

#include "shipequipment/equipment.h"
#include "weapon/projectile.h"

class EquipmentTemplate
{
public:
    static void init();
    
    static sp::P<Equipment> create(sp::string id, sp::P<Ship> target_ship);
    static sp::P<Projectile> createProjectile(sp::string id);
private:
    std::map<sp::string, sp::string> data;

    static std::map<sp::string, EquipmentTemplate> templates;
};

#endif//SHIP_TEMPLATE_H
