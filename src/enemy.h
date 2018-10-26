#ifndef ENEMY_H
#define ENEMY_H

#include "spaceObject.h"

class Projectile;
class Enemy : public SpaceObject
{
public:
    Enemy();
    ~Enemy();

    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source, DamageType type) override;
    virtual void onCollision(sp::CollisionInfo& info) override;
    virtual void onUpdate(float delta) override;
    virtual void onFixedUpdate() override;
    
    static sp::PList<Enemy> bosses;
    
    float getHealth() { return health; }
private:
    bool invincible;
    float health;
    float size;
    float shield;
    float max_shield;
    int damage_indicator;
    int shield_charge_delay;
    int shield_charge_time;
    
    sp::Texture* shielded_texture;
    sp::Texture* unshielded_texture;
    
    int protection_time_on_damage;
    
    float touch_damage_amount;
    DamageType touch_damage_type;
    
    bool glow_enabled;
    float glow_delta;
    float glow_speed;
    sp::Color color;

    void setHealth(float health);
    void setCollisionCircle(float size);
    void setCollisionBox(float width, float height);
    void setShield(float amount, int recharge_time, sp::string texture);
    void setGlow(float speed);
    void disableGlow();
    void setInvincible(bool invincible) { this->invincible = invincible; }
    void setHitProtectionTime(int time) { this->protection_time_on_damage = time; }
    void setBoss() { bosses.add(this); }
    void setDrawOrder(int order) { render_data.order = order; }
    void setTouchDamage(float amount, DamageType type) { touch_damage_amount = amount; touch_damage_type = type; }
    void setColor(float r, float g, float b);
    
    sp::P<Projectile> createProjectile(sp::string name, float x, float y, float angle);

    sp::script::Callback onDestroy;
    sp::script::Callback onDamage;
    sp::script::Callback onPlayerDamage;    //Called when damaging the player
    sp::script::Callback onControlUpdate;
    sp::script::Callback onWeaponUpdate;
    
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
};

#endif//SHIP_H
