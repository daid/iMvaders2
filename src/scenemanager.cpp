#include "scenemanager.h"
#include "spacescene.h"
#include "shipTemplate.h"
#include "keys.h"
#include "playershipcontroller.h"

#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/graphics/gui/widget/progressbar.h>
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>

SceneManager* SceneManager::instance;

SceneManager::SceneManager()
{
    instance = this;

    sp::gui::GraphicsLayer* gui_layer = new sp::gui::GraphicsLayer(100);
    gui_layer->setMinimalVirtualSize(sf::Vector2f(1280, 960));
    gui_layer->setMaximumVirtualSize(sf::Vector2f(1280, 80000));

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
    }else{
        switchToStageSelect();
    }
}
