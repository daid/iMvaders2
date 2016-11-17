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
    
    fire_delay = 0.0;
}

void ProjectileWeapon::setParameter(sp::string key, sp::string value)
{
    if (key == "projectile")
        projectile_name = value;
    else if (key == "rate")
        fire_rate = value.toFloat();
    else if (key == "count")
        projectile_count = value.toInt();
    else if (key == "energy")
        energy_per_shot = value.toFloat();
    else if (key == "angle")
        fire_angle = value.toFloat();
    else if (key == "spread")
        spread_angle = value.toFloat();
    else if (key == "offset")
    {
        spread_offset.x = value.split(",")[0].toFloat();
        spread_offset.y = value.split(",")[1].toFloat();
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
        
        for(int n=0; n<projectile_count; n++)
        {
            double f = 1.0;
            if (projectile_count > 1)
                f = double(n) / double(projectile_count - 1);
            EquipmentTemplate::createProjectile(projectile_name)->launch(ship, spread_offset * -(f * 2.0 - 1.0), fire_angle + -spread_angle / 2.0 + spread_angle * f);
        }
    }
}
