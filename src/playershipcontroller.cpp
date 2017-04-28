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
    
    if (keys->left.get())
        rotate = 5.0;
    else if (keys->right.get())
        rotate = -5.0;
    else
        rotate = 0.0;
    
    brake = keys->brake.get();
    
    primary_fire = keys->primary_fire.get();
    secondary_fire = keys->secondary_fire.get();
}
