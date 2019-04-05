#include "pickup.h"
#include "spacescene.h"
#include "ship.h"
#include "saveData.h"

#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/collision/2d/circle.h>

Pickup::Pickup(sp::Vector2d position)
: sp::Node(space_scene->getRoot())
{
    render_data.shader = sp::Shader::get("internal:basic.shader");
    render_data.type = sp::RenderData::Type::Normal;
    render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1.0, 1.0));
    render_data.texture = sp::texture_manager.get("pickup/pla.png");
    float scale = 1.0;
    render_data.scale = sp::Vector3f(scale, scale, 1.0);
    
    sp::collision::Circle2D shape(0.5);
    shape.type = sp::collision::Shape::Type::Sensor;
    shape.setMaskFilterCategory(SpaceObject::collision_enemy_category);
    shape.setMaskFilterCategory(SpaceObject::collision_projectile_category);

    setCollisionShape(shape);

    setPosition(position);
    setRotation(sp::random(0, 360));
    setLinearVelocity(sp::Vector2d(-5, 0));
    setAngularVelocity(sp::random(300, 500) * (sp::random(0, 100) < 50 ? -1 : 1));
}

void Pickup::onCollision(sp::CollisionInfo& info)
{
    sp::P<Ship> ship = info.other;
    for(int index=0; index<SaveData::instance->player_count; index++)
    {
        if (info.other == StageController::instance->getPlayerShip(index))
        {
            SaveData::instance->player_data[index].pla++;
            delete this;
        }
    }
}

void Pickup::onUpdate(float delta)
{
    if (getGlobalPosition2D().x < -30)
        delete this;
}
