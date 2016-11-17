#include "projectile.h"
#include "../spacescene.h"
#include "../ship.h"
#include <sp2/graphics/spritemanager.h>
#include <sp2/collision/2d/box.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/logging.h>

Projectile::Projectile()
: sp::SceneNode(space_scene->getRoot())
{
    travel_distance = 25.0;
    travel_speed = 25.0;
    lifetime = travel_distance / travel_speed;
}

void Projectile::launch(sp::P<Ship> owner, sp::Vector2d position_offset, double angle_offset)
{
    this->owner = owner;
    
    setRotation(owner->getGlobalRotation2D() + angle_offset);
    sp::Vector2d forward = getGlobalTransform().applyDirection(sp::Vector2d(1, 0));
    setPosition(owner->getGlobalPosition2D() + owner->getGlobalTransform().applyDirection(position_offset));
    setLinearVelocity(forward * travel_speed + owner->getLinearVelocity2D());
}

void Projectile::onUpdate(float delta)
{
    lifetime -= delta;
    if (lifetime <= 0.0)
        delete this;
}

void Projectile::onCollision(sp::CollisionInfo& info)
{
    sp::P<Ship> ship = info.other;
    if (ship && ship != owner)
        delete this;
}

void Projectile::setParameter(sp::string key, sp::string value)
{
    if (key == "sprite")
    {
        render_data = sp::SpriteManager::get(value);
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
    else if (key == "collision")
    {
        std::vector<sp::string> collision = value.split(",");
        if (collision.size() > 1)
        {
            sp::collision::Box2D shape(collision[0].toFloat(), collision[1].toFloat());
            shape.type = sp::collision::Shape::Type::Sensor;
            setCollisionShape(shape);
        }
        else
        {
            sp::collision::Circle2D shape(collision[0].toFloat());
            shape.type = sp::collision::Shape::Type::Sensor;
            setCollisionShape(shape);
        }
    }
    else
    {
        LOG(Warning, "Setting unknown projectile parameter", key, value);
    }
}
