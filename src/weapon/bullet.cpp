#include "bullet.h"
#include "../spacescene.h"
#include "../ship.h"
#include <sp2/graphics/spritemanager.h>
#include <sp2/collision/2d/box.h>

Bullet::Bullet()
: sp::SceneNode(space_scene->getRoot())
{
    sp::collision::Box2D shape(1.0, 0.3);
    shape.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(shape);
    render_data = sp::SpriteManager::get("bullet");
    
    lifetime = distance / speed;
    
}

void Bullet::launch(sp::P<Ship> owner, sp::Vector2d position_offset, double angle_offset)
{
    this->owner = owner;
    
    setRotation(owner->getGlobalRotation2D() + angle_offset);
    sp::Vector2d forward = getGlobalTransform().applyDirection(sp::Vector2d(1, 0));
    setPosition(owner->getGlobalPosition2D() + owner->getGlobalTransform().applyDirection(position_offset));
    setLinearVelocity(forward * speed + owner->getLinearVelocity2D());
}

void Bullet::onUpdate(float delta)
{
    lifetime -= delta;
    if (lifetime <= 0.0)
        delete this;
}

void Bullet::onCollision(sp::CollisionInfo& info)
{
    sp::P<Ship> ship = info.other;
    if (ship && ship != owner)
        delete this;
}
