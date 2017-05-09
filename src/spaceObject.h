#ifndef SPACE_OBJECT_H
#define SPACE_OBJECT_H

#include <sp2/scene/node.h>

class SpaceObject : public sp::Node
{
public:
    enum class DamageSource
    {
        Player,
        Enemy
    };

    SpaceObject();
    ~SpaceObject();
    
    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source) = 0;

    //Collision categories, to reduce the load on collision detection, by filtering away unneeded collisions.
    static constexpr int collision_generic_category = 1;
    static constexpr int collision_player_category = 2;
    static constexpr int collision_enemy_category = 3;
    static constexpr int collision_projectile_category = 4;
};

#endif//SHIP_H
