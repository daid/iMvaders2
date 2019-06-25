#include "stageSelect.h"
#include "keys.h"
#include "saveData.h"
#include "scenemanager.h"

#include <sp2/scene/scene.h>
#include <sp2/scene/camera.h>
#include <sp2/graphics/shader.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/logging.h>
#include <sp2/assert.h>
#include <sp2/graphics/gui/loader.h>

StageSelect::StageSelect()
: sp::Node((new sp::Scene("stage_select"))->getRoot())
{
    sp::P<sp::Camera> camera = new sp::Camera(getParent());
    camera->setPerspective();
    camera->setPosition(sp::Vector3d(0, 0, 50));
    getScene()->setDefaultCamera(camera);

    sp::MeshData::Vertices vertices;
    sp::MeshData::Indices indices{0,1,2,2,1,3};
    vertices.emplace_back(sp::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sp::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sp::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sp::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));
    
    sp::P<sp::Node> background = new sp::Node(getParent());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.shader = sp::Shader::get("shader/background.shader");
    background->render_data.mesh = std::make_shared<sp::MeshData>(std::move(vertices), std::move(indices));
    background->render_data.texture = sp::texture_manager.get("stars.png");
    
    title = new sp::Node(getParent());
    title->render_data.type = sp::RenderData::Type::Normal;
    title->render_data.shader = sp::Shader::get("internal:basic.shader");
    title->render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(60.0, 60.0 / 512 * 135));
    title->render_data.texture = sp::texture_manager.get("logo2.png");
    title->render_data.scale = sp::Vector3f(0.7, 0.7, 0.7);
    title->setPosition(sp::Vector2d(0, 20));

    gui = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_SELECT", nullptr);
}

void StageSelect::show()
{
    wait_for_it = true;
    getScene()->enable();
    gui->show();
    
    SaveData::instance->checkForPlaytroughDone();

    sp::P<sp::gui::Widget> selector = gui->getWidgetWithID("SELECTOR");
    selector->setParent(gui);
    
    sp::P<sp::gui::Widget> stages = gui->getWidgetWithID("STAGES");
    for(auto child : stages->getChildren())
        child.destroy();
    
    sp::P<sp::gui::Widget> row;

    for(int level=1; SaveData::instance->stageExists(level, 0) || SaveData::instance->stageExists(level, 1); level++)
    {
        row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
        if (SaveData::instance->stageExists(level, 0))
        {
            addStageButton(row, level, 0);
        }
        else
        {
            for(int sublevel=1; SaveData::instance->stageExists(level, sublevel); sublevel++)
                addStageButton(row, level, sublevel);
        }
    }

    selection_level = SaveData::instance->unlockedStageLevel() - 1;
    selection_sublevel = 0;

    sp::P<sp::gui::Widget> shop = gui->getWidgetWithID("SHOP");
    if (!SaveData::instance->shop_unlocked)
        shop->hide();
}

void StageSelect::addStageButton(sp::P<sp::gui::Widget> row, int level, int sublevel)
{
    sp::P<sp::gui::Widget> button = sp::gui::Loader::load("gui/stage_select.gui", "STAGE", row->getWidgetWithID("ITEMS"));
    if (level > SaveData::instance->unlockedStageLevel() + 1)
    {
        button->hide();
        return;
    }
    if (level > SaveData::instance->unlockedStageLevel())
    {
        button->disable();
        button->setAttribute("caption", "?");
        return;
    }
    if (SaveData::instance->finished_stages.find(std::pair<int, int>(level, sublevel)) != SaveData::instance->finished_stages.end())
    {
        button->setAttribute("theme_data", "stage_button_done");
    }
    else
    {
        button->setEventCallback([this, level, sublevel](sp::Variant v)
        {
            if (sublevel == 0)
                SceneManager::instance->switchToStage("stage" + sp::string(level));
            else
                SceneManager::instance->switchToStage("stage" + sp::string(level) + "-" + sp::string(sublevel));
            SaveData::instance->finished_stages.emplace(level, sublevel);
        });
    }
    if (sublevel < 1)
        button->setAttribute("caption", sp::string(level));
    else
        button->setAttribute("caption", sp::string(level) + "-" + sp::string(sublevel));
}

void StageSelect::hide()
{
    getScene()->disable();
    gui->hide();
}

void StageSelect::onUpdate(float delta)
{
    if (wait_for_it) { wait_for_it = false; return; }
    
    for(int n=0; n<max_players; n++)
    {
        if (player_keys[n]->down.getDown())
        {
            if (shop_selected)
                shop_selected = false;
            else
                selection_sublevel = selection_sublevel + 1;
        }
        if (player_keys[n]->up.getDown())
        {
            if (shop_selected)
                shop_selected = false;
            else
                selection_sublevel = selection_sublevel - 1;
        }
        if (player_keys[n]->right.getDown() && !shop_selected)
            selection_level = selection_level + 1;
        if (player_keys[n]->left.getDown() && !shop_selected)
            selection_level = selection_level - 1;
    }
    
    
    sp::P<sp::gui::Widget> stage_rows = gui->getWidgetWithID("STAGES");
    int max_stage = std::min(SaveData::instance->unlockedStageLevel() - 1, stage_rows->getChildren().size() - 1);
    if (selection_level < 0)
        selection_level = max_stage;
    if (selection_level > max_stage)
        selection_level = 0;
    
    sp::P<sp::gui::Widget> row;
    sp::P<sp::gui::Widget> item;
    int n=0;
    for(auto child : stage_rows->getChildren())
    {
        if (n == selection_level)
            row = sp::P<sp::Node>(child);
        n++;
    }
    if (row)
    {
        sp::P<sp::gui::Widget> stages = row->getWidgetWithID("ITEMS");
        if (selection_sublevel < 0)
        {
            selection_sublevel = stages->getChildren().size() - 1;
            if (SaveData::instance->shop_unlocked)
                shop_selected = true;
        }
        if (selection_sublevel > stages->getChildren().size() - 1)
        {
            selection_sublevel = 0;
            if (SaveData::instance->shop_unlocked)
                shop_selected = true;
        }

        n=0;
        for(auto child : stages->getChildren())
        {
            if (n == selection_sublevel)
                item = sp::P<sp::Node>(child);
            n++;
        }
    }
    
    if (shop_selected)
    {
        item = gui->getWidgetWithID("SHOP");
    }
    if (item)
    {
        sp::P<sp::gui::Widget> selector = gui->getWidgetWithID("SELECTOR");
        selector->setParent(item);
        if (player_keys[0]->primary_fire.getDown())
            item->onPointerUp(sp::Vector2d(0, 0), -1);
    }

    sp::P<sp::gui::Widget> name = gui->getWidgetWithID("NAME");
    if (shop_selected)
    {
        name->setAttribute("caption", "UltiShop: spend you hard earned polymers");
    }else{
        name->setAttribute("caption", SaveData::instance->getStageTitle(selection_level + 1, selection_sublevel + 1));
    }
}
