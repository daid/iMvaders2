#ifndef SPACE_SCENE_H
#define SPACE_SCENE_H

#include "ship.h"
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/gui/graphicslayer.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/scene/scene.h>

extern sp::P<sp::Scene> space_scene;

class StageController : public sp::SceneNode
{
public:
    StageController();
    
    void show();
    void hide();
    void loadStage(sp::string name);

    virtual void onUpdate(float delta);
private:
    class PlayerData
    {
    public:
        enum class State
        {
            Inactive,
            Playing
        };
        State state;
        sp::P<Ship> ship;
        sp::P<sp::gui::Widget> hud;
    };

    sp::P<sp::CameraNode> camera;
    sp::P<sp::SceneGraphicsLayer> scene_layer;
    
    std::vector<PlayerData> player_data;
    sp::P<sp::SceneNode> background;
};

#endif//SPACE_SCENE
