#include "titleScene.h"
#include "keys.h"
#include "saveData.h"
#include "scenemanager.h"

#include <sp2/scene/scene.h>
#include <sp2/scene/camera.h>
#include <sp2/graphics/shader.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/logging.h>
#include <sp2/assert.h>
#include <sp2/random.h>
#include <sp2/graphics/gui/loader.h>

TitleController::TitleController()
: sp::Node((new sp::Scene("title"))->getRoot())
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
    title->setPosition(sp::Vector2d(0, 15));

    sp::P<sp::Node> text_scroll_root = new sp::Node(getParent());
    text_scroll_root->setPosition(sp::Vector2d(0, -15));
    text_scroll_root->setRotation(sp::Quaterniond::fromAxisAngle(sp::Vector3d(1, 0, 0), -80));

    text_scroll = new sp::Node(text_scroll_root);
    text_scroll->render_data.type = sp::RenderData::Type::Normal;
    text_scroll->render_data.shader = sp::Shader::get("internal:basic.shader");
    text_scroll->render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(71.6, 308));
    text_scroll->render_data.texture = sp::texture_manager.get("scrolltext.png");
    text_scroll->setPosition(sp::Vector2d(0, 0));
    
    title_gui = sp::gui::Loader::load("gui/title.gui", "TITLE", nullptr);
    game_select_gui = sp::gui::Loader::load("gui/title.gui", "GAME_SELECT", nullptr);

    game_select_gui->getWidgetWithID("NEW_GAME_1")->setEventCallback([this](sp::Variant v)
    {
        startNewGame(1);
    });
    game_select_gui->getWidgetWithID("NEW_GAME_2")->setEventCallback([this](sp::Variant v)
    {
        startNewGame(2);
    });
    
    hide();
}

void TitleController::startNewGame(int player_count)
{
    new SaveData(player_count);
    SceneManager::instance->switchToStageSelect();
}

void TitleController::show()
{
    getScene()->enable();
    title_gui->show();
    game_select_gui->hide();
    
    state_time = 0;
    state = State::Scroller;
}

void TitleController::hide()
{
    getScene()->disable();
    title_gui->hide();
    game_select_gui->hide();
}

void TitleController::onUpdate(float delta)
{
    state_time += delta;
    switch(state)
    {
    case State::Scroller:
        {
            if (player_keys[0]->down.get())
                state_time += delta * 5.0;
            if (player_keys[0]->down.get() && player_keys[0]->up.get())
                state_time += delta * 15.0;
            float offset = state_time * text_scroll_speed + text_scroll_start;
            if (offset > text_scroll_end)
                state_time = 0.0;
            if (offset > text_scroll_passed)
            {
                title_gui->show();
                sp::P<sp::gui::Widget> label;
                
                label = title_gui->getWidgetWithID("INSERT");
                if (label)
                {
                    if (fmodf(std::fabs(state_time), 1.0) < 0.5)
                        label->show();
                    else
                        label->hide();
                }
            }
            else
            {
                title_gui->hide();
            }
            game_select_gui->hide();
            
            text_scroll->setPosition(sp::Vector2d(0, offset));
            
            for(int n=0; n<max_players; n++)
            {
                if (player_keys[n]->start.getDown() || player_keys[n]->primary_fire.getDown())
                {
                    state = State::GameSelect;
                    state_time = 0.0;
                    option_index = 0;
                }
            }
        }
        break;
    case State::GameSelect:
        {
            text_scroll->setPosition(sp::Vector2d(0, text_scroll_start));
            title_gui->hide();
            game_select_gui->show();
            
            if (state_time > 10.0)
            {
                state = State::Scroller;
                state_time = 0.0;
            }
            for(int n=0; n<max_players; n++)
            {
                if (player_keys[n]->up.getDown())
                {
                    state_time = fmodf(state_time, 2.0);
                    option_index = (option_index - 1 + 3) % 3;
                }
                if (player_keys[n]->down.getDown())
                {
                    state_time = fmodf(state_time, 2.0);
                    option_index = (option_index + 1) % 3;
                }
            }
            
            game_select_gui->getWidgetWithID("CONTINUE_GAME")->disable();

            sp::P<sp::gui::Widget> selector = game_select_gui->getWidgetWithID("SELECTOR");
            sp::P<sp::gui::Widget> item;
            switch(option_index)
            {
            case 0: item = game_select_gui->getWidgetWithID("NEW_GAME_1"); break;
            case 1: item = game_select_gui->getWidgetWithID("NEW_GAME_2"); break;
            case 2: item = game_select_gui->getWidgetWithID("CONTINUE_GAME"); break;
            }
            if (item)
            {
                for(int n=0; n<max_players; n++)
                {
                    if (player_keys[n]->primary_fire.getDown())
                    {
                        item->onPointerUp(sp::Vector2d(0, 0), -1);
                    }
                }
                selector->setParent(item);
            }
            if (player_keys[0]->start.getDown())
                startNewGame(1);
            if (player_keys[1]->start.getDown())
                startNewGame(2);
        }
        break;
    }
}
