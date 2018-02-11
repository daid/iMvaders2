#include "shield.h"
#include "../ship.h"

Shield::Shield(sp::P<Ship> parent)
: Equipment(parent)
{
    charge_level = max_charge_level = 5.0;
    charge_level = 0.0;
    energy_per_charge = 3.0;
    charge_rate = 1.0;
    charge_delay = 0.0;
}

void Shield::setParameter(sp::string key, sp::string value)
{
    if (key == "charge")
    {
        max_charge_level = sp::stringutil::convert::toFloat(value);
        charge_level = max_charge_level;
    }
    else if (key == "energy")
        energy_per_charge = sp::stringutil::convert::toFloat(value);
    else if (key == "rate")
        charge_rate = sp::stringutil::convert::toFloat(value);
    else
        Equipment::setParameter(key, value);
}

double Shield::takeDamage(double amount)
{
    if (amount < charge_level)
    {
        charge_level -= amount;
        amount = 0;
    }else{
        amount -= charge_level;
        charge_level = 0;
    }
    if (charge_rate > 0.0)
        charge_delay = 1.0 / charge_rate;
    return amount;
}

void Shield::onUpdate(float delta)
{
    if (charge_rate <= 0.0)
        return;
    
    sp::P<Ship> ship = getParent();
    if (charge_delay > 0.0)
    {
        charge_delay -= delta;
    }
    else if (charge_level < max_charge_level)
    {
        if (ship && ship->reactor->useEnergy(energy_per_charge))
        {
            charge_level = std::min(charge_level + 1.0, max_charge_level);
            charge_delay = 1.0 / charge_rate;
        }
    }
}
