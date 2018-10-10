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
    enum class DamageType
    {
        Normal,
        Shield,
        EnergyDrain,
        ReactorDisrupt,
        WeaponDisrupt,
        EngineDisrupt
    };

    SpaceObject();
    ~SpaceObject();
    
    virtual bool takeDamage(sp::Vector2d position, double amount, DamageSource damage_source, DamageType type) = 0;

    //Collision categories, to reduce the load on collision detection, by filtering away unneeded collisions.
    static constexpr int collision_generic_category = 1;
    static constexpr int collision_player_category = 2;
    static constexpr int collision_enemy_category = 3;
    static constexpr int collision_projectile_category = 4;

    void setPosition(sp::Vector2d position);
    sp::Vector2d getPosition();
    void setRotation(float angle);
    float getRotation();
    void destroy();
    
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
};

namespace sp::script {
    static inline SpaceObject::DamageType convertFromLua(lua_State* L, typeIdentifier<SpaceObject::DamageType>, int index)
    {
        sp::string str = luaL_checkstring(L, index);
        str = str.lower();
        if (str == "normal")
            return SpaceObject::DamageType::Normal;
        if (str == "shield")
            return SpaceObject::DamageType::Shield;
        if (str.startswith("energy"))
            return SpaceObject::DamageType::EnergyDrain;
        else if (str.startswith("reactor"))
            return SpaceObject::DamageType::ReactorDisrupt;
        else if (str.startswith("weapon"))
            return SpaceObject::DamageType::WeaponDisrupt;
        else if (str.startswith("engine"))
            return SpaceObject::DamageType::EngineDisrupt;
        luaL_error(L, "Unkown damage type [%s]", luaL_checkstring(L, index));
        return SpaceObject::DamageType::Normal;
    }
}

#endif//SHIP_H
