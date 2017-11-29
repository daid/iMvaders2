#ifndef REACTOR_H
#define REACTOR_H

#include "equipment.h"

class Reactor : public Equipment
{
public:
    Reactor(sp::P<Ship> parent);
    
    float getEnergyLevel() const { return energy_level; }
    float getMaxEnergyLevel() const { return max_energy_level; }
    bool useEnergy(double amount);
    void drainEnergy(double amount);
    
    virtual void onUpdate(float delta) override;
    virtual void setParameter(sp::string key, sp::string value) override;
private:
    double energy_level;
    
    double max_energy_level;
    double charge_rate;
};

#endif//REACTOR_H
