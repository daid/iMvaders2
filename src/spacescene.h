#ifndef SPACE_SCENE_H
#define SPACE_SCENE_H

#include "ship.h"
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/gui/graphicslayer.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/scene/scene.h>
#include <sp2/script/environment.h>

extern sp::P<sp::Scene> space_scene;

class StageController : public sp::Node
{
public:
    static StageController* instance;

    StageController();
    
    void show();
    void hide();
    void loadStage(sp::string name);
    void stageDone();

    virtual void onUpdate(float delta) override;
    virtual void onFixedUpdate() override;
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

    void updateHud(PlayerData& data);

    sp::P<sp::CameraNode> camera;
    sp::P<sp::SceneGraphicsLayer> scene_layer;
    
    std::vector<PlayerData> player_data;
    sp::P<sp::Node> background;
    sp::P<sp::gui::Widget> message;
    sp::P<sp::gui::Widget> main_hud;
    
    sp::Vector2d background_speed;
    
    sp::P<sp::script::Environment> script;
};

#endif//SPACE_SCENE
