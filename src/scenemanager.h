#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "titleScene.h"
#include "stageSelect.h"
#include "spacescene.h"
#include "ship.h"
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/gui/graphicslayer.h>

class SceneManager
{
public:
    static SceneManager* instance;

    SceneManager();

    void switchToTitle();
    void switchToStageSelect();
    void switchToStage(std::string stage_name);

private:
    TitleController* title_controller;
    StageSelect* stage_select;
    StageController* stage_controller;
};

#endif//SCENE_MANAGER_H
