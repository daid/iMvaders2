#include "hull.h"
#include "../ship.h"

Hull::Hull(sp::P<Ship> parent)
: Equipment(parent)
{
    hull_level = max_hull_level = 5;
}

void Hull::setParameter(sp::string key, sp::string value)
{
    if (key == "damage")
        hull_level = max_hull_level = sp::stringutil::convert::toFloat(value);
    else
        Equipment::setParameter(key, value);
}

bool Hull::takeDamage(double amount)
{
    hull_level -= amount;
    if (hull_level <= 0.0)
        return true;
    return false;
}

void Hull::repair()
{
    hull_level = max_hull_level;
}
