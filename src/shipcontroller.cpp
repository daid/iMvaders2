#include "shipcontroller.h"

ShipController::ShipController()
{
    trust = sp::Vector2d(0, 0);
    rotate = 0;
    primary_fire = false;
    secondary_fire = false;
    brake = false;
}
