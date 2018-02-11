#ifndef SPACE_SCENE_H
#define SPACE_SCENE_H

#include "ship.h"
#include <sp2/scene/camera.h>
#include <sp2/graphics/gui/scene.h>
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
    bool loadStage(sp::string name);
    void stageDone();

    virtual void onUpdate(float delta) override;
    virtual void onFixedUpdate() override;
    
    sp::P<Ship> getPlayerShip(int index);
private:
    class PlayerData
    {
    public:
        enum class State
        {
            Inactive,
            Playing
        };
        int index;
        State state;
        sp::P<Ship> ship;
        sp::P<sp::gui::Widget> hud;
    };

    void updateHud(PlayerData& data);

    sp::P<sp::Camera> camera;
    
    std::vector<PlayerData> player_data;
    sp::P<sp::Node> background;
    sp::P<sp::gui::Widget> message;
    sp::P<sp::gui::Widget> main_hud;
    
    sp::Vector2d background_speed;
    
    sp::P<sp::script::Environment> script;
    
    float game_over_delay;
    bool stage_done;
    float stage_done_time;
};

#endif//SPACE_SCENE
