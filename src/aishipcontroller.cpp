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
        ship->getScene()->queryCollision2D(ship->getGlobalPosition2D(), 25, [this, ship](sp::P<sp::SceneNode> node)
        {
            if (node != ship && sp::P<Ship>(node))
            {
                target = node;
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
        
        primary_fire = true;
    }
}
