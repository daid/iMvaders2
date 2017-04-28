#include "spacescene.h"
#include "keys.h"
#include "saveData.h"
#include "ship.h"
#include "playershipcontroller.h"
#include "shipTemplate.h"

#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/graphics/gui/widget/progressbar.h>
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>

sp::P<sp::Scene> space_scene;

StageController::StageController()
: sp::SceneNode((new sp::Scene("space"))->getRoot())
{
    space_scene = getScene();
    
    camera = new sp::CameraNode(space_scene->getRoot());
    camera->setOrtographic(15.0);
    camera->setRotation(-90);
    camera->setPosition(sp::Vector2d(10, 0));

    scene_layer = new sp::SceneGraphicsLayer(10);
    scene_layer->addRenderPass(new sp::BasicNodeRenderPass("window", space_scene, camera));
#ifdef DEBUG
    //scene_layer->addRenderPass(new sp::CollisionRenderPass("window", space_scene, camera));
#endif

    for(int n=0; n<max_players; n++)
    {
        PlayerData pd;
        pd.state = PlayerData::State::Inactive;
        pd.hud = sp::gui::Loader::load("gui/hud.gui", "HUD", nullptr);
        if (n == 1)
        {
            for(auto widget : pd.hud->children)
            {
                if (widget->layout.alignment == sp::gui::Widget::Alignment::BottomLeft)
                    widget->layout.alignment = sp::gui::Widget::Alignment::BottomRight;
                if (widget->layout.alignment == sp::gui::Widget::Alignment::Left)
                    widget->layout.alignment = sp::gui::Widget::Alignment::Right;
                if (widget->layout.alignment == sp::gui::Widget::Alignment::TopLeft)
                    widget->layout.alignment = sp::gui::Widget::Alignment::TopRight;
            }
        }
        pd.hud->hide();

        player_data.push_back(pd);
    }

    std::vector<sp::MeshData::Vertex> vertices;
    vertices.emplace_back(sf::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));

    background = new sp::SceneNode(space_scene->getRoot());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.shader = sp::Shader::get("shader/star_background.shader");
    background->render_data.mesh = std::make_shared<sp::MeshData>(vertices);
    background->render_data.texture = "stars.png";
    background->render_data.order = -1;
    background->render_data.color = sf::Color::White;
}

void StageController::show()
{
    space_scene->enable();
}

void StageController::hide()
{
    space_scene->disable();
}

void StageController::loadStage(sp::string name)
{
    LOG(Info, "Going to load stage:", name);
    for(auto child : space_scene->getRoot()->getChildren())
    {
        if (child != *camera && child != *background && child != this)
            delete child;
    }
    
    for(int index=0; index<SaveData::instance->player_count; index++)
    {
        PlayerData& data = player_data[index];
        if ((index % 2) == 0)
            data.ship = ShipTemplate::create("UM-M");
        else
            data.ship = ShipTemplate::create("UM-U");
        data.ship->setRotation(0);
        data.ship->setPosition(sp::Vector2d(-12, 0));
        data.ship->controller = new PlayerShipController(index);
    }
}

void StageController::onUpdate(float delta)
{
    int ship_count = 0;
    sp::Vector2d position_accumulator;
    for(unsigned int n=0; n<max_players; n++)
    {
        if (player_data[n].ship)
        {
            position_accumulator += player_data[n].ship->getGlobalPosition2D();
            ship_count++;
        }
    }
    if (ship_count > 0)
        camera->setPosition(position_accumulator / double(ship_count) * 0.2);
}
/*
void SceneManager::onUpdate(float delta)
{
    for(unsigned int n=0; n<max_players; n++)
    {
        PlayerData& data = player_data[n];
        if (data.state == PlayerData::State::Inactive && (player_keys[n]->start.get() || player_keys[n]->primary_fire.get()))
        {
            activatePlayer(n);
        }
        
        sp::P<sp::gui::Progressbar> bar;
        bar = data.hud->getWidgetWithID("ENERGY");
        if (bar)
        {
            if (data.ship && data.ship->reactor)
            {
                bar->setValue(data.ship->reactor->getEnergyLevel());
                bar->setRange(0, data.ship->reactor->getMaxEnergyLevel());
            }
            else
            {
                bar->setValue(0);
            }
        }
        bar = data.hud->getWidgetWithID("SHIELD");
        if (bar)
        {
            if (data.ship && data.ship->shield)
            {
                bar->setValue(data.ship->shield->getChargeLevel());
                bar->setRange(0, data.ship->shield->getMaxChargeLevel());
            }
            else
            {
                bar->setValue(0);
            }
        }
        bar = data.hud->getWidgetWithID("HULL");
        if (bar)
        {
            if (data.ship && data.ship->hull)
            {
                bar->setValue(data.ship->hull->getHullLevel());
                bar->setRange(0, data.ship->hull->getMaxHullLevel());
            }
            else
            {
                bar->setValue(0);
            }
        }
    }
    
    int ship_count = 0;
    sp::Vector2d position_accumulator;
    for(unsigned int n=0; n<max_players; n++)
    {
        if (player_data[n].ship)
        {
            position_accumulator += player_data[n].ship->getGlobalPosition2D();
            ship_count++;
        }
    }
    if (ship_count > 0)
        camera->setPosition(position_accumulator / double(ship_count) * 0.2);
}

void SceneManager::activatePlayer(int index)
{
    PlayerData& data = player_data[index];
    if (data.state != PlayerData::State::Inactive)
        return;
    
    data.state = PlayerData::State::Playing;
    
    if ((index % 2) == 0)
        data.ship = ShipTemplate::create("UM-M");
    else
        data.ship = ShipTemplate::create("UM-U");
    data.ship->faction = new Faction();
    data.ship->setRotation(0);
    data.ship->setPosition(sp::Vector2d(-12, 0));
    data.ship->controller = new PlayerShipController(index);

    updateViews();
}

void SceneManager::updateViews()
{
    int player_count = 0;
    for(unsigned int n=0; n<max_players; n++)
    {
        if (player_data[n].state != PlayerData::State::Inactive)
            player_count++;
    }

    if (player_count == 0)
    {
        space_scene->disable();
        title_scene->enable();
    }
    else
    {
        title_scene->disable();
        space_scene->enable();

        scene_layer->enable();
    }
}
*/
