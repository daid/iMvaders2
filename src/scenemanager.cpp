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

SceneManager::SceneManager()
{
    title_scene = new TitleScene();
    
    for(int n=0; n<max_players; n++)
    {
        PlayerData pd;
        pd.state = PlayerData::State::Inactive;
        pd.gui_layer = new sp::gui::GraphicsLayer(100);
        pd.gui_layer->setMinimalVirtualSize(sf::Vector2f(1280, 800));
        pd.gui_layer->setMaximumVirtualSize(sf::Vector2f(1280, 80000));
        pd.gui_layer->disable();
        pd.hud = sp::gui::Loader::load("gui/hud.gui", "HUD", pd.gui_layer->getRoot());

        pd.camera = new sp::CameraNode(space_scene->getRoot());
        pd.camera->setOrtographic(25.0);
        pd.camera->setRotation(-90);
        pd.camera->setPosition(sp::Vector2d(10, 0));

        pd.scene_layer = new sp::SceneGraphicsLayer(10);
        pd.scene_layer->addRenderPass(new sp::BasicNodeRenderPass("window", space_scene, pd.camera));
#ifdef DEBUG
        if (n == 0) pd.scene_layer->addRenderPass(new sp::CollisionRenderPass("window", space_scene, pd.camera));
#endif

        player_data.push_back(pd);
    }

    std::vector<sp::MeshData::Vertex> vertices;
    vertices.emplace_back(sf::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));

    sp::P<sp::SceneNode> background = new sp::SceneNode(space_scene->getRoot());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.shader = sp::Shader::get("shader/star_background.shader");
    background->render_data.mesh = std::make_shared<sp::MeshData>(vertices);
    background->render_data.texture = "stars.png";
    background->render_data.order = -1;
    background->render_data.color = sf::Color::White;
}

void SceneManager::onUpdate(float delta)
{
    for(unsigned int n=0; n<max_players; n++)
    {
        PlayerData& data = player_data[n];
        if (data.state == PlayerData::State::Inactive && (player_keys[n]->start.get() || player_keys[n]->primary_fire.get()))
        {
            activatePlayer(n);
        }
        
        if (data.ship)
        {
            sp::P<sp::gui::Progressbar> bar;
            bar = data.gui_layer->getRoot()->getWidgetWithID("ENERGY");
            if (bar)
            {
                bar->setValue(data.ship->reactor->getEnergyLevel());
                bar->setRange(0, data.ship->reactor->getMaxEnergyLevel());
            }
            bar = data.gui_layer->getRoot()->getWidgetWithID("SHIELD");
            if (bar)
            {
                bar->setValue(data.ship->shield->getChargeLevel());
                bar->setRange(0, data.ship->shield->getMaxChargeLevel());
            }
            bar = data.gui_layer->getRoot()->getWidgetWithID("HULL");
            if (bar)
            {
                bar->setValue(data.ship->hull->getHullLevel());
                bar->setRange(0, data.ship->hull->getMaxHullLevel());
            }
            
            data.camera->setPosition(data.ship->getGlobalPosition2D() + data.ship->getLinearVelocity2D() * 0.1);
        }
    }
}

void SceneManager::activatePlayer(int index)
{
    PlayerData& data = player_data[index];
    if (data.state != PlayerData::State::Inactive)
        return;
    
    data.state = PlayerData::State::Playing;
    
    if (index == 1)
        data.ship = ShipTemplate::create("MAKERBOT-M");
    else
        data.ship = ShipTemplate::create("UM-M");
    data.ship->setRotation(sp::random(0, 360));
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
        
        int player_nr = 0;
        for(unsigned int n=0; n<max_players; n++)
        {
            if (player_data[n].state != PlayerData::State::Inactive)
            {
                updateViewForPlayer(n, player_nr, player_count);
                player_nr++;
                player_data[n].gui_layer->enable();
                player_data[n].scene_layer->enable();
            }
            else
            {
                player_data[n].gui_layer->disable();
                player_data[n].scene_layer->disable();
            }
        }
    }
}

void SceneManager::updateViewForPlayer(int index, int player_nr, int player_count)
{
    PlayerData& data = player_data[index];
    sf::FloatRect viewport(0, 0, 1, 1);
    switch(player_count)
    {
    case 1:
        viewport = sf::FloatRect(0, 0, 1, 1);
        break;
    case 2:
        switch(player_nr)
        {
        case 0:
            viewport = sf::FloatRect(0, 0, 0.5, 1);
            break;
        case 1:
            viewport = sf::FloatRect(0.5, 0, 0.5, 1);
            break;
        }
        break;
    case 3:
    case 4:
        switch(player_nr)
        {
        case 0:
            viewport = sf::FloatRect(0, 0, 0.5, 0.5);
            break;
        case 1:
            viewport = sf::FloatRect(0.5, 0, 0.5, 0.5);
            break;
        case 2:
            viewport = sf::FloatRect(0, 0.5, 0.5, 0.5);
            break;
        case 3:
            viewport = sf::FloatRect(0.5, 0.5, 0.5, 0.5);
            break;
        }
        break;
    }
    data.gui_layer->setViewport(viewport);
    data.scene_layer->setViewport(viewport);
}
