#include "equipment.h"
#include "../ship.h"
#include <sp2/logging.h>

Equipment::Equipment(sp::P<Ship> parent)
: sp::SceneNode(parent)
{
    mass = 0.0;
}

void Equipment::setParameter(sp::string key, sp::string value)
{
    if (key == "id")
        id = value;
    else if (key == "name")
        name = value;
    else if (key == "mass")
        mass = value.toFloat();
    else
        LOG(Warning, "Setting unknown equipment parameter", key, value);
}