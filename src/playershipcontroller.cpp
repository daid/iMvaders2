#include "playershipcontroller.h"
#include "ship.h"

PlayerShipController::PlayerShipController(int index)
{
    keys = player_keys[index];
}

void PlayerShipController::update(Ship* ship)
{
    trust.x = keys->up.getValue() - keys->down.getValue();
    trust.y = keys->left.getValue() - keys->right.getValue();
    if (keys->up.get())
    {
        if (keys->left.get())
            rotate = 45.0;
        else if (keys->right.get())
            rotate = 315.0;
        else
            rotate = 0.0;
    }
    else if (keys->down.get())
    {
        if (keys->left.get())
            rotate = 135.0;
        else if (keys->right.get())
            rotate = 225.0;
        else
            rotate = 180.0;
    }
    else if (keys->left.get())
        rotate = 90.0;
    else if (keys->right.get())
        rotate = 270.0;
    else
        rotate = ship->getGlobalRotation2D();
    brake = keys->brake.get();
    
    primary_fire = keys->primary_fire.get();
    secondary_fire = keys->secondary_fire.get();
}
