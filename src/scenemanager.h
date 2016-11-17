#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "titleScene.h"
#include "ship.h"
#include <sp2/graphics/gui/graphicslayer.h>

class SceneManager : public sp::Scene
{
public:
    SceneManager();

    virtual void onUpdate(float delta) override;
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
        sp::P<sp::gui::GraphicsLayer> gui_layer;
        sp::P<sp::SceneGraphicsLayer> scene_layer;
        sp::P<Ship> ship;
        sp::P<sp::CameraNode> camera;
        sp::P<sp::gui::Widget> hud;
    };
    
    std::vector<PlayerData> player_data;

    sp::P<TitleScene> title_scene;
    
    void activatePlayer(int index);
    void updateViews();
    void updateViewForPlayer(int index, int player_nr, int player_count);
};

#endif//SCENE_MANAGER_H
