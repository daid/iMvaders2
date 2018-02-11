#include "reactor.h"
#include "../ship.h"

Reactor::Reactor(sp::P<Ship> parent)
: Equipment(parent)
{
    energy_level = max_energy_level = 10.0;
    charge_rate = 2.0;
}

void Reactor::setParameter(sp::string key, sp::string value)
{
    if (key == "energy")
        energy_level = max_energy_level = sp::stringutil::convert::toFloat(value);
    else if (key == "rate")
        charge_rate = sp::stringutil::convert::toFloat(value);
    else
        Equipment::setParameter(key, value);
}

bool Reactor::useEnergy(double amount)
{
    if (energy_level < amount)
        return false;
    energy_level -= amount;
    return true;
}

void Reactor::drainEnergy(double amount)
{
    energy_level -= amount;
    if (energy_level < 0)
        energy_level = 0;
}

void Reactor::onUpdate(float delta)
{
    energy_level = std::min(max_energy_level, energy_level + charge_rate * delta);
}
