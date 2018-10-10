#include "spaceObject.h"
#include "spacescene.h"

SpaceObject::SpaceObject()
: sp::Node(space_scene->getRoot())
{
}

SpaceObject::~SpaceObject()
{
}

void SpaceObject::setPosition(sp::Vector2d position)
{
    sp::Node::setPosition(position);
}

sp::Vector2d SpaceObject::getPosition()
{
    return getGlobalPosition2D();
}

void SpaceObject::setRotation(float angle)
{
    sp::Node::setRotation(angle);
}

float SpaceObject::getRotation()
{
    return getGlobalRotation2D();
}

void SpaceObject::destroy()
{
    delete this;
}

void SpaceObject::onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class)
{
    sp::Node::onRegisterScriptBindings(script_binding_class);

    script_binding_class.bind("setPosition", &setPosition);
    script_binding_class.bind("getPosition", &getPosition);
    script_binding_class.bind("setRotation", &setRotation);
    script_binding_class.bind("getRotation", &getRotation);
    script_binding_class.bind("destroy", &destroy);
}
