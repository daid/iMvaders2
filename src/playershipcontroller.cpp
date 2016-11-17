#include "playershipcontroller.h"

PlayerShipController::PlayerShipController(int index)
{
    keys = player_keys[index];
}

void PlayerShipController::update(Ship* ship)
{
    trust.x = keys->up.getValue() - keys->down.getValue();
    if (keys->strafe.get())
    {
        trust.y = keys->left.getValue() - keys->right.getValue();
        rotate = 0.0;
    }else{
        trust.y = 0.0;
        rotate = keys->left.getValue() - keys->right.getValue();
    }
    brake = keys->brake.get();
    
    primary_fire = keys->primary_fire.get();
    secondary_fire = keys->secondary_fire.get();
}
