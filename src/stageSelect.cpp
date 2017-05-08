#include "stageSelect.h"
#include "keys.h"
#include "saveData.h"
#include "scenemanager.h"

#include <sp2/scene/scene.h>
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/spriteManager.h>
#include <sp2/graphics/shader.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/logging.h>
#include <sp2/assert.h>
#include <sp2/graphics/gui/guiLoader.h>

StageSelect::StageSelect()
: sp::Node((new sp::Scene("stage_select"))->getRoot())
{
    sp::P<sp::CameraNode> camera = new sp::CameraNode(getParent());
    camera->setPerspective();
    camera->setPosition(sp::Vector3d(0, 0, 50));
    getScene()->setDefaultCamera(camera);

    std::vector<sp::MeshData::Vertex> vertices;
    vertices.emplace_back(sf::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));
    
    sp::P<sp::Node> background = new sp::Node(getParent());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.shader = sp::Shader::get("shader/background.shader");
    background->render_data.mesh = std::make_shared<sp::MeshData>(vertices);
    background->render_data.texture = "stars.png";
    background->render_data.color = sf::Color::White;
    
    title = new sp::Node(getParent());
    title->render_data = sp::SpriteManager::get("logo2");
    title->render_data.scale = sf::Vector3f(0.7, 0.7, 0.7);
    title->setPosition(sp::Vector2d(0, 20));

    //Build the renderer for this specific scene.
    graphics_layer = new sp::SceneGraphicsLayer(1);
    graphics_layer->addRenderPass(new sp::BasicNodeRenderPass("window", getScene()));

    gui = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_SELECT", nullptr);
}

void StageSelect::show()
{
    getScene()->enable();
    gui->show();
    
    sp::P<sp::gui::Widget> stages = gui->getWidgetWithID("STAGES");
    for(auto child : stages->children)
        delete child;
    
    sp::P<sp::gui::Widget> row;

    row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
    addStageButton(row, 1, 0);

    row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
    addStageButton(row, 2, 1);
    addStageButton(row, 2, 2);

    row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
    addStageButton(row, 3, 0);

    row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
    addStageButton(row, 4, 1);
    addStageButton(row, 4, 2);

    row = sp::gui::Loader::load("gui/stage_select.gui", "STAGE_ROW", stages);
    addStageButton(row, 5, 0);

    selection_level = SaveData::instance->unlockedStageLevel() - 1;
    selection_sublevel = 0;
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
            SaveData::instance->finished_stages.emplace(level, sublevel);
            if (sublevel == 0)
                SceneManager::instance->switchToStage("stage" + sp::string(level));
            else
                SceneManager::instance->switchToStage("stage" + sp::string(level) + "-" + sp::string(sublevel));
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
    for(int n=0; n<max_players; n++)
    {
        if (player_keys[n]->up.getDown())
            selection_sublevel = selection_sublevel + 1;
        if (player_keys[n]->down.getDown())
            selection_sublevel = selection_sublevel - 1;
        if (player_keys[n]->right.getDown())
            selection_level = selection_level + 1;
        if (player_keys[n]->left.getDown())
            selection_level = selection_level - 1;
    }
    
    
    sp::P<sp::gui::Widget> stage_rows = gui->getWidgetWithID("STAGES");
    int max_stage = std::min(SaveData::instance->unlockedStageLevel() - 1, stage_rows->children.size() - 1);
    if (selection_level < 0)
        selection_level = max_stage;
    if (selection_level > max_stage)
        selection_level = 0;
    
    sp::P<sp::gui::Widget> row;
    sp::P<sp::gui::Widget> item;
    int n=0;
    for(auto child : stage_rows->children)
    {
        if (n == selection_level)
            row = child;
        n++;
    }
    if (row)
    {
        sp::P<sp::gui::Widget> stages = row->getWidgetWithID("ITEMS");
        if (selection_sublevel < 0)
            selection_sublevel = stages->children.size() - 1;
        if (selection_sublevel > stages->children.size() - 1)
            selection_sublevel = 0;

        n=0;
        for(auto child : stages->children)
        {
            if (n == selection_sublevel)
                item = child;
            n++;
        }
    }
    if (item)
    {
        sp::P<sp::gui::Widget> selector = gui->getWidgetWithID("SELECTOR");
        selector->layout.position.x = item->layout.rect.left - selector->layout.size.x;
        selector->layout.position.y = item->layout.rect.top - gui->layout.margin_top;
        if (player_keys[0]->primary_fire.getDown())
            item->onPointerUp(sp::Vector2f(item->layout.rect.left, item->layout.rect.top), -1);
    }

    sp::P<sp::gui::Widget> name = gui->getWidgetWithID("NAME");
    switch(selection_level)
    {
    case 0:
        name->setAttribute("caption", "STAGE 1: Welcome back to the jungle");
        break;
    case 1:
        switch(selection_sublevel)
        {
        case 0: name->setAttribute("caption", "STAGE 2-1: For the lulz"); break;
        case 1: name->setAttribute("caption", "STAGE 2-2: Blast from the past"); break;
        }
        break;
    case 2:
        break;
    }
}
