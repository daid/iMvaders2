#include "projectileWeapon.h"
#include "../equipmentTemplate.h"
#include "../ship.h"
#include <sp2/logging.h>
#include <sp2/engine.h>

ProjectileWeapon::ProjectileWeapon(sp::P<Ship> parent)
: Weapon(parent)
{
    projectile_name = "PULSE";
    fire_rate = 5.0;
    projectile_count = 1;
    energy_per_shot = 0.1;
    fire_angle = 0.0;
    spread_angle = 0.0;
    spread_offset = sp::Vector2d(0, 0);
    burst_count = 0;
    burst_time = 0.0;
    
    fire_delay = 0.5;
    projectile_burst_count = 0.0;
}

void ProjectileWeapon::setParameter(sp::string key, sp::string value)
{
    if (key == "projectile")
        projectile_name = value;
    else if (key == "rate")
        fire_rate = sp::stringutil::convert::toFloat(value);
    else if (key == "count")
        projectile_count = sp::stringutil::convert::toInt(value);
    else if (key == "energy")
        energy_per_shot = sp::stringutil::convert::toFloat(value);
    else if (key == "angle")
        fire_angle = sp::stringutil::convert::toFloat(value);
    else if (key == "spread")
        spread_angle = sp::stringutil::convert::toFloat(value);
    else if (key == "offset")
    {
        spread_offset = sp::stringutil::convert::toVector2d(value);
    }
    else if (key == "burst")
    {
        burst_count = sp::stringutil::convert::toInt(value.split(",")[0]);
        burst_time = sp::stringutil::convert::toFloat(value.split(",")[1]);
    }
    else
        Equipment::setParameter(key, value);
}

void ProjectileWeapon::onFixedUpdate()
{
    sp::P<Ship> ship = getParent();

    if (fire_delay > 0)
        fire_delay -= sp::Engine::fixed_update_delta;

    while(fire && fire_delay <= 0.0 && ship->reactor->useEnergy(energy_per_shot))
    {
        fire_delay += 1.0 / fire_rate;
        
        launchProjectiles();
        if (burst_count > 0)
        {
            projectile_burst_count = burst_count - 1;
            projectile_burst_delay = burst_time / burst_count;
        }
    }
    if (projectile_burst_count > 0)
    {
        projectile_burst_delay -= sp::Engine::fixed_update_delta;
        if (projectile_burst_delay <= 0.0)
        {
            projectile_burst_count -= 1;
            projectile_burst_delay = burst_time / burst_count;
            launchProjectiles();
        }
    }
}

void ProjectileWeapon::launchProjectiles()
{
    sp::P<Ship> ship = getParent();

    for(int n=0; n<projectile_count; n++)
    {
        double f = 1.0;
        if (projectile_count > 1)
            f = double(n) / double(projectile_count - 1);
        EquipmentTemplate::createProjectile(projectile_name)->launch(ship, spread_offset * -(f * 2.0 - 1.0), fire_angle + -spread_angle / 2.0 + spread_angle * f, SpaceObject::DamageSource::Player);
    }
}

bool ProjectileWeapon::hasHitPotential(sp::P<Ship> target)
{
    sp::P<Ship> ship = getParent();
    
    sp::Vector2d position_diff = target->getGlobalPosition2D() - ship->getGlobalPosition2D();
    double target_rotation = position_diff.angle();
    if (std::abs(sp::angleDifference(target_rotation, ship->getGlobalRotation2D())) < 15)
        return true;
    return false;
}
