#include "abandondedPrinter.h"
#include "../ship.h"
#include "../explosion.h"
#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/box.h>

AbandondedPrinter::AbandondedPrinter()
{
    render_data.shader = sp::Shader::get("shader/basic.shader");
    render_data.type = sp::RenderData::Type::Normal;
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1.0, 1.0));
    if (sp::random(0, 100) < 50)
        render_data.texture = "ship/m/cupcake.png";
    else
        render_data.texture = "ship/m/thingomatic.png";
    float scale = 4.0;
    render_data.scale = sp::Vector3f(scale, scale, 1.0);

    sp::collision::Box2D shape(3, 4);
    shape.type = sp::collision::Shape::Type::Dynamic;
    shape.density = 5;
    shape.setFilterCategory(collision_enemy_category);
    setCollisionShape(shape);

    setPosition(sp::Vector2d(20, sp::random(-20, 20)));
    setRotation(sp::random(0, 360));
    setLinearVelocity(sp::Vector2d(sp::random(-2.5, -3.5), sp::random(-0.3, 0.3)));
    setAngularVelocity(sp::random(-30, 30));
    
    health = 20;
}

bool AbandondedPrinter::takeDamage(sp::Vector2d position, double amount, DamageSource damage_source)
{
    if (damage_source != DamageSource::Player)
        return false;
    health -= amount;
    if (health <= 0.0)
    {
        new Explosion(getGlobalPosition2D(), 2);
        delete this;
    }
    return true;
}

void AbandondedPrinter::onCollision(sp::CollisionInfo& info)
{
    sp::P<Ship> ship = info.other;
    if (ship)
    {
        if (info.force > 10)
        {
            ship->takeDamage(info.position, 1.0, SpaceObject::DamageSource::Enemy);
        }
    }
}

void AbandondedPrinter::onFixedUpdate()
{
    sp::Vector2d position = getGlobalPosition2D();
    if (position.x < -25 || position.x > 25 || position.y < -30 || position.y > 30)
        delete this;
}
