#ifndef AI_SHIP_CONTROLLER_H
#define AI_SHIP_CONTROLLER_H

#include "shipcontroller.h"
#include "keys.h"

class AIShipController : public ShipController
{
public:
    AIShipController();

    virtual void update(Ship* ship) override;

private:
    sp::P<Ship> target;
};

#endif//AI_SHIP_CONTROLLER_H
