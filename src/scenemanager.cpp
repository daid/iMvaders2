#include "scenemanager.h"
#include "spacescene.h"
#include "shipTemplate.h"
#include "keys.h"
#include "playershipcontroller.h"

#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/gui/loader.h>
#include <sp2/graphics/gui/widget/progressbar.h>
#include <sp2/scene/camera.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>

SceneManager* SceneManager::instance;

SceneManager::SceneManager()
{
    instance = this;

    new sp::gui::Scene(sp::Vector2d(1280, 960));

    title_controller = new TitleController();
    stage_select = new StageSelect();
    stage_controller = new StageController();

    switchToTitle();
}

void SceneManager::switchToTitle()
{
    title_controller->show();
    stage_select->hide();
    stage_controller->hide();
}

void SceneManager::switchToStageSelect()
{
    title_controller->hide();
    stage_select->show();
    stage_controller->hide();
}

void SceneManager::switchToStage(std::string stage_name)
{
    if (stage_controller->loadStage(stage_name))
    {
        title_controller->hide();
        stage_select->hide();
        stage_controller->show();
    }
}
