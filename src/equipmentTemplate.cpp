#include "equipmentTemplate.h"
#include "shipequipment/reactor.h"
#include "shipequipment/engine.h"
#include "shipequipment/shield.h"
#include "shipequipment/hull.h"
#include "weapon/projectileWeapon.h"

#include <sp2/logging.h>
#include <sp2/io/keyValueTreeLoader.h>

std::map<sp::string, EquipmentTemplate> EquipmentTemplate::templates;

void EquipmentTemplate::init()
{
    sp::P<sp::KeyValueTree> tree = sp::io::KeyValueTreeLoader::load("equipment.txt");
    for(auto it : tree->getFlattenNodesByIds())
    {
        const sp::string& id = it.first;
        std::map<sp::string, sp::string>& data = it.second;

        EquipmentTemplate et;
        et.data = data;
        
        templates[id] = et;
    }
    delete *tree;
}

sp::P<Equipment> EquipmentTemplate::create(sp::string id, sp::P<Ship> target_ship)
{
    auto it = templates.find(id);
    if (it == templates.end())
    {
        LOG(Error, "Tried to create non existing equipment:", id);
        return nullptr;
    }
    EquipmentTemplate& et = it->second;
    sp::string type = et.data["type"];
    sp::P<Equipment> equipment;
    if (type == "reactor")
        equipment = new Reactor(target_ship);
    else if (type == "engine")
        equipment = new Engine(target_ship);
    else if (type == "shield")
        equipment = new Shield(target_ship);
    else if (type == "hull")
        equipment = new Hull(target_ship);
    else if (type == "projectile_weapon")
        equipment = new ProjectileWeapon(target_ship);
    for(auto i : et.data)
    {
        if (i.first != "type")
            equipment->setParameter(i.first, i.second);
    }
    return equipment;
}
