#ifndef HULL_H
#define HULL_H

#include "equipment.h"

class Hull : public Equipment
{
public:
    Hull(sp::P<Ship> parent);
    
    double getHullLevel() const { return hull_level; }
    void setHullLevel(double hull) { hull_level = std::min(max_hull_level, hull); }
    double getMaxHullLevel() const { return max_hull_level; }
    bool takeDamage(double amount);
    void repair();
    
    virtual void setParameter(sp::string key, sp::string value) override;
private:
    double hull_level;
    
    double max_hull_level;
};

#endif//SHIELD_H
