#include "pickup.h"
#include "spacescene.h"
#include "ship.h"
#include "saveData.h"

#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/collision/2d/circle.h>

Pickup::Pickup(sp::Vector2d position)
: sp::Node(space_scene->getRoot())
{
    render_data.shader = sp::Shader::get("shader/basic.shader");
    render_data.type = sp::RenderData::Type::Normal;
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1.0, 1.0));
    render_data.texture = "pickup/pla.png";
    float scale = 1.0;
    render_data.scale = sp::Vector3f(scale, scale, 1.0);
    
    sp::collision::Circle2D shape(0.5);
    shape.type = sp::collision::Shape::Type::Sensor;
    setCollisionShape(shape);

    setPosition(position);
    setRotation(sp::random(0, 360));
    setLinearVelocity(sp::Vector2d(-5, 0));
    setAngularVelocity(sp::random(300, 500) * (sp::random(0, 100) < 50 ? -1 : 1));
}

void Pickup::onCollision(sp::CollisionInfo& info)
{
    sp::P<Ship> ship = info.other;
    if (ship)
    {
        SaveData::instance->pla++;
        delete this;
    }
}