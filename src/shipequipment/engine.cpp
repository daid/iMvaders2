#include "engine.h"
#include "../ship.h"
#include <sp2/logging.h>
#include <sp2/engine.h>

Engine::Engine(sp::P<Ship> parent)
: Equipment(parent)
{
    forward_trust = 30.0;
    reverse_trust = 10.0;
    rotation_trust = 10.0;
    strafe_trust = 10.0;
    energy_per_trust = 0.01;
}

void Engine::setParameter(sp::string key, sp::string value)
{
    if (key == "forward")
        forward_trust = value.toFloat();
    else if (key == "reverse")
        reverse_trust = value.toFloat();
    else if (key == "rotation")
        rotation_trust = value.toFloat();
    else if (key == "strafe")
        strafe_trust = value.toFloat();
    else if (key == "energy")
        energy_per_trust = value.toFloat();
    else
        Equipment::setParameter(key, value);
}

void Engine::onFixedUpdate()
{
    sp::P<Ship> ship = getParent();

    sp::Vector2d linear_velocity = ship->getLinearVelocity2D();
    double angular_velocity = ship->getAngularVelocity2D();
    linear_velocity *= 0.98;
    angular_velocity *= 0.8;
    if (ship->controller->brake)
    {
        linear_velocity *= 0.9;
        angular_velocity *= 0.6;
    }
    sp::Vector2d trust_request;
    if (ship->controller->trust.x > 0.0)
        trust_request.x = ship->controller->trust.x * forward_trust * sp::Engine::fixed_update_delta;
    if (ship->controller->trust.x < 0.0)
        trust_request.x = ship->controller->trust.x * reverse_trust * sp::Engine::fixed_update_delta;
    trust_request.y = ship->controller->trust.y * strafe_trust * sp::Engine::fixed_update_delta;

    double angular_request = ship->controller->rotate * rotation_trust * sp::Engine::fixed_update_delta;
    
    if (!ship->reactor->useEnergy(std::abs(trust_request.x * energy_per_trust)))
        trust_request.x = 0.0;
    if (!ship->reactor->useEnergy(std::abs(trust_request.y * energy_per_trust)))
        trust_request.y = 0.0;
    if (!ship->reactor->useEnergy(std::abs(angular_request * energy_per_trust)))
        angular_request = 0.0;
    
    linear_velocity += getGlobalTransform().applyDirection(trust_request);
    
    angular_velocity += angular_request / sp::pi * 180.0;
    
    ship->setLinearVelocity(linear_velocity);
    ship->setAngularVelocity(angular_velocity);
}
