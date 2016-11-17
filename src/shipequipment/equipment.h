#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <sp2/scene/node.h>

class Ship;
class Equipment : public sp::SceneNode
{
public:
    Equipment(sp::P<Ship> parent);
    
    virtual void setParameter(sp::string key, sp::string value);
private:
    sp::string id;
    sp::string name;
};

#endif//EQUIPMENT_H

