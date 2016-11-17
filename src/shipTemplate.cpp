#include "shipTemplate.h"
#include <sp2/graphics/spriteManager.h>
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
        st.collision_size.x = collision[0].toFloat();
        if (collision.size() > 1)
            st.collision_size.y = collision[1].toFloat();
        else
            st.collision_size.y = -1;
        
        sp::SpriteManager::create(st.sprite_name, data["sprite"], data["size"].toFloat());
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
    ship->render_data = sp::SpriteManager::get(st.sprite_name);
    if (st.collision_size.y > 0)
        ship->setCollisionShape(sp::collision::Box2D(st.collision_size.x, st.collision_size.y));
    else
        ship->setCollisionShape(sp::collision::Circle2D(st.collision_size.x));
    
    return ship;
}
