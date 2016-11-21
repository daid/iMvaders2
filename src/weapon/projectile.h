#ifndef WEAPON_PROJECTILE_H
#define WEAPON_PROJECTILE_H

#include <sp2/scene/node.h>

class Ship;
class Projectile : public sp::SceneNode
{
public:
    Projectile();

    void launch(sp::P<Ship> owner, sp::Vector2d position_offset, double angle_offset);
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    
    virtual void setParameter(sp::string key, sp::string value);
private:
    double travel_distance;
    double travel_speed;
    double lifetime;
    double damage;

    sp::P<Ship> owner;
};

#endif//WEAPON_BULLET_H
