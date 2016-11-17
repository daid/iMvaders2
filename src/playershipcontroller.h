#ifndef PLAYER_SHIP_CONTROLLER_H
#define PLAYER_SHIP_CONTROLLER_H

#include "shipcontroller.h"
#include "keys.h"

class PlayerShipController : public ShipController
{
public:
    PlayerShipController(int index);

    virtual void update(Ship* ship) override;

private:
    PlayerKeys* keys;
};

#endif//PLAYER_SHIP_CONTROLLER_H
