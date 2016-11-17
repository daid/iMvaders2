#ifndef WEAPON_BULLET_H
#define WEAPON_BULLET_H

#include <sp2/scene/node.h>

class Ship;
class Bullet : public sp::SceneNode
{
public:
    Bullet();

    void launch(sp::P<Ship> owner, sp::Vector2d position_offset, double angle_offset);
    
    virtual void onUpdate(float delta) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
private:
    static constexpr double distance = 25.0;
    static constexpr double speed = 25.0;
    
    float lifetime;
    sp::P<Ship> owner;
};

#endif//WEAPON_BULLET_H
