#include "shield.h"
#include "../ship.h"

Shield::Shield(sp::P<Ship> parent)
: Equipment(parent)
{
    charge_level = max_charge_level = 5.0;
    charge_level = 0.0;
    energy_per_charge = 3.0;
    charge_rate = 1.0;
}

void Shield::setParameter(sp::string key, sp::string value)
{
    if (key == "charge")
        max_charge_level = value.toFloat();
    else if (key == "energy")
        energy_per_charge = value.toFloat();
    else if (key == "rate")
        charge_rate = value.toFloat();
    else
        Equipment::setParameter(key, value);
}

double Shield::takeDamage(double amount)
{
    return amount;
}

void Shield::onUpdate(float delta)
{
    sp::P<Ship> ship = getParent();
    double charge_delta = charge_rate * delta;
    charge_delta = std::min(charge_delta, max_charge_level - charge_level);
    if (ship && ship->reactor->useEnergy(charge_delta * energy_per_charge))
        charge_level = std::min(max_charge_level, charge_level + charge_delta);
}
