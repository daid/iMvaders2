#include "projectile.h"
#include "../spacescene.h"
#include "../ship.h"
#include "../enemy.h"
#include "../explosion.h"
#include <sp2/graphics/spriteManager.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/logging.h>

Projectile::Projectile()
: sp::Node(space_scene->getRoot())
{
    travel_distance = 25.0;
    travel_speed = 25.0;
    damage = 1.0;
    lifetime = travel_distance / travel_speed;
    type = SpaceObject::DamageType::Normal;
    destroy_on_hit = true;
}

void Projectile::launch(sp::P<sp::Node> owner, sp::Vector2d position_offset, double angle_offset, SpaceObject::DamageSource damage_source)
{
    this->damage_source = damage_source;
    
    setRotation(owner->getGlobalRotation2D() + angle_offset);
    sp::Vector2d forward = getGlobalTransform().applyDirection(sp::Vector2d(1, 0));
    setPosition(owner->getGlobalPosition2D() + owner->getGlobalTransform().applyDirection(position_offset));
    setLinearVelocity(forward * travel_speed);
}

void Projectile::onUpdate(float delta)
{
    lifetime -= delta;
    if (lifetime <= 0.0)
        delete this;
}

void Projectile::onCollision(sp::CollisionInfo& info)
{
    sp::P<SpaceObject> so = info.other;
    if (so)
    {
        if (so->takeDamage(info.position, damage, damage_source, type))
        {
            if (destroy_on_hit)
            {
                new Explosion(info.position, 0.3, getLinearVelocity2D() * 0.1);
                delete this;
            }
        }
    }
}

void Projectile::setParameter(sp::string key, sp::string value)
{
    if (key == "sprite")
    {
        render_data = sp::SpriteManager::get(value);
        render_data.order = 1;
    }
    else if (key == "size")
    {
    }
    else if (key == "distance")
    {
        travel_distance = value.toFloat();
        lifetime = travel_distance / travel_speed;
    }
    else if (key == "speed")
    {
        travel_speed = value.toFloat();
        if (travel_distance > 0.0)
            lifetime = travel_distance / travel_speed;
    }
    else if (key == "lifetime")
    {
        lifetime = value.toFloat();
        travel_distance = 0.0;
    }
    else if (key == "damage")
    {
        damage = value.toFloat();
    }
    else if (key == "type")
    {
        value = value.lower();
        if (value == "normal")
            type = SpaceObject::DamageType::Normal;
        else if (value == "shield")
            type = SpaceObject::DamageType::Shield;
        else if (value.startswith("energy"))
            type = SpaceObject::DamageType::EnergyDrain;
        else if (value.startswith("reactor"))
            type = SpaceObject::DamageType::ReactorDisrupt;
        else if (value.startswith("weapon"))
            type = SpaceObject::DamageType::WeaponDisrupt;
        else if (value.startswith("engine"))
            type = SpaceObject::DamageType::EngineDisrupt;
        else
            LOG(Warning, "Unknown projectile damage type:", value);
    }
    else if (key == "collision")
    {
        std::vector<sp::string> collision = value.split(",");
        if (collision.size() > 1)
        {
            sp::collision::Box2D shape(collision[0].toFloat(), collision[1].toFloat());
            shape.type = sp::collision::Shape::Type::Sensor;
            shape.setFilterCategory(SpaceObject::collision_projectile_category);
            shape.setMaskFilterCategory(SpaceObject::collision_projectile_category);
            setCollisionShape(shape);
        }
        else
        {
            sp::collision::Circle2D shape(collision[0].toFloat());
            shape.type = sp::collision::Shape::Type::Sensor;
            shape.setFilterCategory(SpaceObject::collision_projectile_category);
            shape.setMaskFilterCategory(SpaceObject::collision_projectile_category);
            setCollisionShape(shape);
        }
    }
    else if (key == "destroy")
    {
        destroy_on_hit = value.lower() == "true";
    }
    else
    {
        LOG(Warning, "Setting unknown projectile parameter", key, value);
    }
}
