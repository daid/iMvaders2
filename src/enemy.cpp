#include "enemy.h"
#include "spacescene.h"
#include "ship.h"
#include "equipmentTemplate.h"
#include "explosion.h"

#include <sp2/random.h>
#include <sp2/tween.h>
#include <sp2/collision/2d/circle.h>
#include <sp2/collision/2d/box.h>

Enemy::Enemy()
{
    setRotation(180);
    setPosition(-50, -50);
    health = 1.0;
    size = 1.0;
    damage_indicator = 0;
    protection_time_on_damage = 20;
    
    invincible = false;
    shield = max_shield = 0;
    shield_charge_delay = 0;
    shield_charge_time = 0;

    glow_enabled = false;
    glow_delta = 0.0;
    glow_speed = 1.0;
}

Enemy::~Enemy()
{
}

void Enemy::setHealth(float health)
{
    this->health = health;
}

void Enemy::setPosition(float x, float y)
{
    sp::Node::setPosition(sp::Vector2d(x, y));
}

sp::Vector2d Enemy::getPosition()
{
    return getGlobalPosition2D();
}

void Enemy::setRotation(float angle)
{
    sp::Node::setRotation(angle);
}

float Enemy::getRotation()
{
    return getGlobalRotation2D();
}

void Enemy::onUpdate(float delta)
{
    if (glow_enabled)
    {
        glow_delta += glow_speed * delta;
        if (glow_delta >= 1.0)
            glow_delta -= 1.0;
        if (glow_delta < 0.5)
            render_data.color.r = render_data.color.g = render_data.color.b = sp::Tween<int>::linear(glow_delta, 0.0, 0.5, 255, 0);
        else
            render_data.color.r = render_data.color.g = render_data.color.b = sp::Tween<int>::linear(glow_delta, 0.5, 1.0, 0, 255);
    }else{
        render_data.color.r = render_data.color.g = render_data.color.b = 255;
    }
}

void Enemy::onFixedUpdate()
{
    if (damage_indicator > 0)
        damage_indicator -= 1;
    if (shield_charge_time > 0)
    {
        shield_charge_time -= 1;
    }
    else if (shield < max_shield)
    {
        shield = max_shield;
        render_data.texture = shielded_texture;
    }

    onControlUpdate.call(this);
    onWeaponUpdate.call(this);
    
    if (damage_indicator > 0)
        render_data.color.a = (damage_indicator % 4) < 2 ? 255 : 0;
    else
        render_data.color.a = 255;
}

bool Enemy::takeDamage(sp::Vector2d position, double amount, DamageSource damage_source)
{
    if (damage_source == DamageSource::Enemy)
        return false;
    if (invincible)
        return true;
    if (damage_indicator > 0)
        return true;
    if (shield > 0)
    {
        shield -= amount;
        if (shield <= 0)
        {
            render_data.texture = unshielded_texture;
        }
    }
    else
    {
        health -= amount;
        damage_indicator = protection_time_on_damage;
    }
    shield_charge_time = shield_charge_delay;
    onDamage.call(amount);
    if (health <= 0.0)
    {
        float es = size * 0.3;
        for(int n=0; n<5; n++)
        {   
            new Explosion(getGlobalPosition2D() + sp::Vector2d(sp::random(-es, es), sp::random(-es, es)), es);
        }
        onDestroy.call(this);
        delete this;
    }
    return true;
}

void Enemy::onCollision(sp::CollisionInfo& info)
{
    sp::P<SpaceObject> so = info.other;
    if (so)
    {
        so->takeDamage(info.position, 1.0, SpaceObject::DamageSource::Enemy);
    }
}

void Enemy::setCollisionCircle(float size)
{
    sp::collision::Circle2D shape(size);
    shape.type = sp::collision::Shape::Type::Sensor;
    shape.setFilterCategory(collision_enemy_category);
    shape.setMaskFilterCategory(collision_enemy_category);
    setCollisionShape(shape);
    
    this->size = size;
}

void Enemy::setCollisionBox(float width, float height)
{
    sp::collision::Box2D shape(width, height);
    shape.type = sp::collision::Shape::Type::Sensor;
    shape.setFilterCategory(collision_enemy_category);
    shape.setMaskFilterCategory(collision_enemy_category);
    setCollisionShape(shape);
    
    this->size = std::sqrt(width * width + height * height);
}

void Enemy::setShield(float amount, int recharge_time, sp::string texture)
{
    shield = amount;
    max_shield = amount;
    
    shield_charge_delay = recharge_time;
    shield_charge_time = 0;
    
    unshielded_texture = render_data.texture;
    shielded_texture = texture;
    
    render_data.texture = texture;
}

void Enemy::setGlow(float speed)
{
    if (!glow_enabled)
        glow_delta = 0.0;
    glow_enabled = true;
    glow_speed = speed;
}

void Enemy::disableGlow()
{
    glow_enabled = false;
}

sp::P<Projectile> Enemy::createProjectile(sp::string name, float x, float y, float angle)
{
    sp::P<Projectile> projectile = EquipmentTemplate::createProjectile(name);
    projectile->launch(this, sp::Vector2d(x, y), angle, SpaceObject::DamageSource::Enemy);
    return projectile;
}

void Enemy::onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class)
{
    sp::Node::onRegisterScriptBindings(script_binding_class);

    script_binding_class.bind("setHealth", setHealth);
    script_binding_class.bind("setPosition", setPosition);
    script_binding_class.bind("getPosition", getPosition);
    script_binding_class.bind("setRotation", setRotation);
    script_binding_class.bind("getRotation", getRotation);
    script_binding_class.bind("setCollisionCircle", setCollisionCircle);
    script_binding_class.bind("setCollisionBox", setCollisionBox);
    script_binding_class.bind("setShield", setShield);
    script_binding_class.bind("setGlow", setGlow);
    script_binding_class.bind("disableGlow", disableGlow);
    script_binding_class.bind("setInvincible", setInvincible);
    script_binding_class.bind("createProjectile", createProjectile);

    script_binding_class.bind("onDestroy", onDestroy);
    script_binding_class.bind("onDamage", onDamage);
    script_binding_class.bind("onControlUpdate", onControlUpdate);
    script_binding_class.bind("onWeaponUpdate", onWeaponUpdate);
}
