#include "z18.h"
#include "../explosion.h"
#include "../pickup.h"
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/collision/2d/box.h>

class Z18Part : public SpaceObject
{
public:
    Z18Part(Z18* owner, int index)
    : owner(owner), index(index)
    {
        render_data.shader = sp::Shader::get("shader/basic.shader");
        render_data.type = sp::RenderData::Type::Normal;
        render_data.texture = sp::texture_manager.get("ship/m/z18.png");
        render_data.order = -1;

        setPosition(sp::Vector2d(60, -(index - 24) + 0.5));
        
        updateToSize();
    }

    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source, DamageType type) override
    {
        if (damage_source != SpaceObject::DamageSource::Player)
            return false;
        owner->applyDamage(position, amount);
        return true;
    }
    
    virtual void onCollision(sp::CollisionInfo& info) override
    {
        sp::P<SpaceObject> so = info.other;
        if (so)
        {
            so->takeDamage(info.position, 1.0, SpaceObject::DamageSource::Enemy, DamageType::Normal);
        }
    }
    
    virtual void onFixedUpdate()
    {
        setPosition(getPosition2D() - sp::Vector2d(Z18::speed, 0));
    }
    
    void updateToSize()
    {
        float s0 = owner->size[index];
        float s1 = owner->size[index+1];
        float s = (s0 + s1) / 2;
        if (s0 <= 0.6 || s1 <= 0.6)
        {
            owner->parts[index] = nullptr;
            delete this;
            return;
        }
    
        sp::MeshData::Vertices vertices;
        sp::MeshData::Indices indices{0,1,2,3,4,5};
        float uv_y = 0.5 + (1.0/80.0) * (index - 24);
        vertices.emplace_back(sp::Vector3f( 40,           -0.5, 0.0f), sp::Vector2f(1.0, uv_y + 1.0/80.0));
        vertices.emplace_back(sp::Vector3f( 40 - s0,  0.5, 0.0f), sp::Vector2f(1.0-s0/80.0, uv_y));
        vertices.emplace_back(sp::Vector3f( 40 - s1, -0.5, 0.0f), sp::Vector2f(1.0-s1/80.0, uv_y + 1.0/80.0));
        
        vertices.emplace_back(sp::Vector3f( 40 - s0,  0.5, 0.0f), sp::Vector2f(1.0-s0/80.0, uv_y));
        vertices.emplace_back(sp::Vector3f( 40, -0.5, 0.0f), sp::Vector2f(1, uv_y + 1.0/80.0));
        vertices.emplace_back(sp::Vector3f( 40,  0.5, 0.0f), sp::Vector2f(1, uv_y));

        render_data.shader = sp::Shader::get("shader/basic.shader");
        render_data.type = sp::RenderData::Type::Normal;
        render_data.mesh = sp::MeshData::create(std::move(vertices), std::move(indices));
        render_data.texture = sp::texture_manager.get("ship/m/z18.png");
        render_data.order = -1;

        sp::collision::Box2D shape(s, 1.0, 0.5 * (80.0 - s), 0);
        shape.type = sp::collision::Shape::Type::Sensor;
        shape.setFilterCategory(collision_enemy_category);
        shape.setMaskFilterCategory(collision_enemy_category);
        setCollisionShape(shape);
    }
    
    Z18* owner;
    int index;
};

Z18::Z18()
{
    for(int n=0;n<part_count+1;n++)
        size[n] = 80.0;
    for(int n=0;n<part_count;n++)
        parts[n] = new Z18Part(this, n);
    setPosition(sp::Vector2d(60, 0));
    pickup_at_total_size = 75;
}

bool Z18::takeDamage(sp::Vector2d position, double amount, SpaceObject::DamageSource damage_source, DamageType type)
{
    if (damage_source != SpaceObject::DamageSource::Player)
        return false;
    return true;
}

void Z18::onCollision(sp::CollisionInfo& info)
{
    sp::P<SpaceObject> so = info.other;
    if (so)
    {
        so->takeDamage(info.position, 1.0, SpaceObject::DamageSource::Enemy, DamageType::Normal);
    }
}

void Z18::onFixedUpdate()
{
    setPosition(getPosition2D() - sp::Vector2d(speed, 0));
    if (getPosition2D().x < -60)
    {
        for(int n=0;n<part_count;n++)
            if (parts[n])
                delete parts[n];
        delete this;
    }
}

void Z18::applyDamage(sp::Vector2d position, double amount)
{
    int index = int(-position.y + 24.5);
    float hit_size = ((getGlobalPosition2D().x - position.x) + 40.0);
    int s = 8;
    for(int n=index-s;n<index+s+1;n++)
    {
        if (n >= 0 && n < part_count)
        {
            float y_diff = std::fabs((float(-n) + 25.0) - position.y);
            float hole_size = std::max(0.0f, 4.0f - y_diff);
            float want_size_a = hit_size - hole_size;
            float want_size_b = size[n] - hole_size;
            size[n] = std::min(size[n], std::max(want_size_a, want_size_b));
        }
    }
    for(int n=index-s-1;n<=index+s+2;n++)
    {
        if (n >= 0 && n < part_count && parts[n])
        {
            parts[n]->updateToSize();
        }
    }
    new Explosion(position, 4.0);

    float total_size = 0.0;
    for(int n=0;n<part_count+1;n++)
    {
        total_size += size[n];
    }
    total_size /= part_count + 1;
    if (total_size < pickup_at_total_size)
    {
        new Pickup(position);
        pickup_at_total_size -= 5;
    }
}
