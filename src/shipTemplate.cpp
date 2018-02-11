#include "shipTemplate.h"
#include <sp2/io/keyValueTreeLoader.h>
#include <sp2/logging.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/collision/2d/box.h>

std::map<sp::string, ShipTemplate> ShipTemplate::templates;

void ShipTemplate::init()
{
    sp::P<sp::KeyValueTree> tree = sp::io::KeyValueTreeLoader::load("ships.txt");
    for(auto it : tree->getFlattenNodesByIds())
    {
        const sp::string& id = it.first;
        std::map<sp::string, sp::string>& data = it.second;

        ShipTemplate st;
        st.name = data["name"];
        st.sprite_name = "ship-" + id;
        std::vector<sp::string> collision = data["collision"].split(",");
        st.collision_size.x = sp::stringutil::convert::toFloat(collision[0]);
        if (collision.size() > 1)
            st.collision_size.y = sp::stringutil::convert::toFloat(collision[1]);
        else
            st.collision_size.y = -1;

        st.reactor = data["reactor"];
        st.engine = data["engine"];
        st.shield = data["shield"];
        st.hull = data["hull"];
        st.weapon[0] = data["primary_weapon"];
        st.weapon[1] = data["secondary_weapon"];

        //TODO:sp::SpriteManager::create(st.sprite_name, data["sprite"], sp::stringutil::convert::toFloat(data["size"]));
        templates[id] = st;
    }
    delete *tree;
}

sp::P<Ship> ShipTemplate::create(sp::string id)
{
    auto it = templates.find(id);
    if (it == templates.end())
    {
        LOG(Error, "Tried to create non existing ship template:", id);
        return nullptr;
    }
    ShipTemplate& st = it->second;
    sp::P<Ship> ship = new Ship();
    //TODO:ship->render_data = sp::SpriteManager::get(st.sprite_name);
    if (st.collision_size.y > 0)
    {
        sp::collision::Box2D shape(st.collision_size.x, st.collision_size.y);
        shape.setFilterCategory(SpaceObject::collision_player_category);
        shape.setMaskFilterCategory(SpaceObject::collision_player_category);
        ship->setCollisionShape(shape);
    }
    else
    {
        sp::collision::Circle2D shape(st.collision_size.x);
        shape.setFilterCategory(SpaceObject::collision_player_category);
        shape.setMaskFilterCategory(SpaceObject::collision_player_category);
        ship->setCollisionShape(shape);
    }
    
    ship->changeReactor(st.reactor);
    ship->changeEngine(st.engine);
    ship->changeShield(st.shield);
    ship->changeHull(st.hull);
    ship->changeWeapon(0, st.weapon[0]);
    ship->changeWeapon(1, st.weapon[1]);
    
    return ship;
}
