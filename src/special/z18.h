#ifndef Z18_H
#define Z18_H

#include "../spaceObject.h"

class Z18Part;
class Z18 : public SpaceObject
{
public:
    Z18();
    
    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source, DamageType type) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onFixedUpdate() override;
    
    void applyDamage(sp::Vector2d position, double amount);

    static constexpr int part_count = 48;
    static constexpr float speed = 0.15;
    Z18Part* parts[part_count];
    float size[part_count+1];
    float pickup_at_total_size;
};

#endif//Z18_H
