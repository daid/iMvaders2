#ifndef SHIELD_H
#define SHIELD_H

#include "equipment.h"

class Shield : public Equipment
{
public:
    Shield(sp::P<Ship> parent);
    
    float getChargeLevel() const { return charge_level; }
    float getMaxChargeLevel() const { return max_charge_level; }
    double takeDamage(double amount);
    
    virtual void onUpdate(float delta) override;
    virtual void setParameter(sp::string key, sp::string value) override;
private:
    double charge_level;
    
    double max_charge_level;
    double energy_per_charge;
    double charge_rate;
};

#endif//SHIELD_H
