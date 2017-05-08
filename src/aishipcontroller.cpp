#include "aishipcontroller.h"
#include "ship.h"

#include <sp2/scene/scene.h>
#include <sp2/logging.h>

AIShipController::AIShipController()
{
}

void AIShipController::update(Ship* ship)
{
    trust.x = 0;
    trust.y = 0;
    rotate = ship->getGlobalRotation2D();
    brake = true;
    
    primary_fire = false;
    secondary_fire = false;
    
    if (!target)
    {
        ship->getScene()->queryCollision(ship->getGlobalPosition2D(), 25, [this, ship](sp::P<sp::Node> node)
        {
            sp::P<Ship> other = node;
            if (other && other->faction != ship->faction)
            {
                target = other;
                return false;
            }
            return true;
        });
    }
    
    if (target)
    {
        trust = sp::normalize(target->getGlobalPosition2D() - ship->getGlobalPosition2D());
        brake = false;
        rotate = sp::toRotationAngle(target->getGlobalPosition2D() - ship->getGlobalPosition2D());
        
        if (ship->weapon[0] && ship->weapon[0]->hasHitPotential(target))
            primary_fire = true;
        if (ship->weapon[1] && ship->weapon[1]->hasHitPotential(target))
            secondary_fire = true;
    }
}
