#ifndef PROJECTILE_WEAPON_H
#define PROJECTILE_WEAPON_H

#include "weapon.h"

class ProjectileWeapon : public Weapon
{
public:
    ProjectileWeapon(sp::P<Ship> parent);
    
    virtual void onFixedUpdate() override;
    virtual void setParameter(sp::string key, sp::string value) override;
private:
    double fire_delay;
    
    sp::string projectile_name;
    double fire_rate;
    double energy_per_shot;
    double fire_angle;
    double spread_angle;
    sp::Vector2d spread_offset;
    
    int projectile_count;
};

#endif//SHIELD_H
