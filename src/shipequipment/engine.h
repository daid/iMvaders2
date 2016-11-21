#ifndef ENGINE_H
#define ENGINE_H

#include "equipment.h"

class Engine : public Equipment
{
public:
    Engine(sp::P<Ship> parent);
    
    virtual void onFixedUpdate() override;
    virtual void setParameter(sp::string key, sp::string value) override;
private:
    double forward_trust;   //forward trust in u/s^2
    double rotation_trust;  //rotation trust in radians/s
    double energy_per_trust;//energy use per u/s^2
};

#endif//SHIELD_H
