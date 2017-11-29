#ifndef WEAPON_PROJECTILE_H
#define WEAPON_PROJECTILE_H

#include <sp2/scene/node.h>
#include "../faction.h"
#include "../spaceObject.h"

class Ship;
class Projectile : public sp::Node
{
public:
    Projectile();

    void launch(sp::P<sp::Node> owner, sp::Vector2d position_offset, double angle_offset, SpaceObject::DamageSource damage_source);
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    
    virtual void setParameter(sp::string key, sp::string value);
private:
    double travel_distance;
    double travel_speed;
    double lifetime;
    double damage;
    bool destroy_on_hit;
    
    SpaceObject::DamageType type;
    SpaceObject::DamageSource damage_source;
};

#endif//WEAPON_BULLET_H
