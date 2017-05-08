#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <sp2/scene/node.h>

class Ship;
class Equipment : public sp::Node
{
public:
    Equipment(sp::P<Ship> parent);
    
    virtual void setParameter(sp::string key, sp::string value);

    double mass;

    sp::string id;
    sp::string name;
};

#endif//EQUIPMENT_H

