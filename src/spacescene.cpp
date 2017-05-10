#include "spacescene.h"
#include "keys.h"
#include "saveData.h"
#include "ship.h"
#include "playershipcontroller.h"
#include "shipTemplate.h"
#include "enemy.h"
#include "message.h"
#include "scenemanager.h"
#include "pickup.h"
#include "special/z18.h"

#include <sp2/random.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/gui/guiLoader.h>
#include <sp2/graphics/gui/widget/progressbar.h>
#include <sp2/graphics/gui/widget/label.h>
#include <sp2/graphics/gui/widget/image.h>
#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>

sp::P<sp::Scene> space_scene;
static int global_time;

StageController* StageController::instance;

StageController::StageController()
: sp::Node((new sp::Scene("space"))->getRoot())
{
    instance = this;
    space_scene = getScene();
    
    camera = new sp::CameraNode(space_scene->getRoot());
    camera->setOrtographic(15.0);
    camera->setRotation(-90);
    camera->setPosition(sp::Vector2d(10, 0));

    scene_layer = new sp::SceneGraphicsLayer(10);
    scene_layer->addRenderPass(new sp::BasicNodeRenderPass("window", space_scene, camera));
#ifdef DEBUG
    scene_layer->addRenderPass(new sp::CollisionRenderPass("window", space_scene, camera));
#endif

    for(int n=0; n<max_players; n++)
    {
        PlayerData pd;
        pd.state = PlayerData::State::Inactive;
        pd.hud = sp::gui::Loader::load("gui/hud.gui", "PLAYER", nullptr);
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
    main_hud = sp::gui::Loader::load("gui/hud.gui", "MAIN", nullptr);
    message = sp::gui::Loader::load("gui/hud.gui", "MESSAGE", nullptr);

    std::vector<sp::MeshData::Vertex> vertices;
    vertices.emplace_back(sf::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));

    background = new sp::Node(space_scene->getRoot());
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
    message->hide();
    for(int n=0; n<max_players; n++)
        player_data[n].hud->hide();
    for(int n=0; n<SaveData::instance->player_count; n++)
        player_data[n].hud->show();
    main_hud->show();
}

void StageController::hide()
{
    space_scene->disable();
    message->hide();
    for(int n=0; n<max_players; n++)
    {
        player_data[n].hud->hide();
    }
    main_hud->hide();
}

static int luaf_include(lua_State* lua)
{
    sp::string filename = luaL_checkstring(lua, 1);

    auto resource = sp::io::ResourceProvider::get("stages/" + filename);
    if (!resource)
    {
        return luaL_error(lua, "Failed to find %s", filename);
    }

    sp::string filecontents = resource->readAll();

    if (luaL_loadbuffer(lua, filecontents.c_str(), filecontents.length(), filename.c_str()))
    {
        return lua_error(lua);
    }
    
    //set the environment table it as 1st upvalue for the loaded chunk, else it works in a different environment then where this function was called from.
    lua_pushvalue(lua, lua_upvalueindex(1));
    lua_setupvalue(lua, -2, 1);

    lua_call(lua, 0, 0);
    return 0;
}

static sp::P<Message> createMessage(sp::string message)
{
    Message* m = new Message();
    m->setText(message);
    return m;
}

static sp::P<Enemy> createEnemy(sp::string texture_name, float scale)
{
    Enemy* e = new Enemy();
    e->render_data.shader = sp::Shader::get("shader/basic.shader");
    e->render_data.type = sp::RenderData::Type::Normal;
    e->render_data.mesh = sp::MeshData::createQuad(sp::Vector2f(1.0, 1.0));
    e->render_data.texture = texture_name;
    e->render_data.scale = sp::Vector3f(scale, scale, 1.0);
    return e;
}

static sp::P<Pickup> createPickup(float x, float y)
{
    return new Pickup(sp::Vector2d(x, y));
}

static sp::P<SpaceObject> createSpecial(sp::string name)
{
    if (name == "Z18")
        return new Z18();
    return nullptr;
}

static int getGlobalTime()
{
    return global_time;
}

static int getPlaCount()
{
    return SaveData::instance->pla;
}

static void stageDone()
{
    StageController::instance->stageDone();
}

void StageController::stageDone()
{
    stage_done = true;
    stage_done_time = 0;
}

bool StageController::loadStage(sp::string name)
{
    LOG(Info, "Going to load stage:", name);
    global_time = 0;
    stage_done = false;
    for(auto child : space_scene->getRoot()->getChildren())
    {
        if (child != *camera && child != *background && child != this)
            delete child;
    }
    background->setPosition(sp::Vector2d(0, 0));
    background_speed = sp::Vector2d(-5, 0);
    
    for(int index=0; index<SaveData::instance->player_count; index++)
    {
        PlayerData& data = player_data[index];
        SaveData::PlayerData& save_data = SaveData::instance->player_data[index];
        
        if (save_data.hull <= 0.0)
            continue;
        
        if ((index % 2) == 0)
            data.ship = ShipTemplate::create("UM-M");
        else
            data.ship = ShipTemplate::create("UM-U");
        data.ship->setRotation(0);
        data.ship->setPosition(sp::Vector2d(-12, sp::random(-5, 5)));
        data.ship->controller = new PlayerShipController(index);
        data.ship->hull->setHullLevel(data.ship->hull->getMaxHullLevel() * save_data.hull);
    }
    
    if (script)
        delete *script;
    script = new sp::script::Environment();
    script->setGlobal("include", luaf_include);
    script->setGlobal("createMessage", createMessage);
    script->setGlobal("createEnemy", createEnemy);
    script->setGlobal("createPickup", createPickup);
    script->setGlobal("createSpecial", createSpecial);
    script->setGlobal("getGlobalTime", getGlobalTime);
    script->setGlobal("getPlaCount", getPlaCount);
    script->setGlobal("random", sp::random);
    script->setGlobal("stageDone", ::stageDone);
    if (!script->load(sp::io::ResourceProvider::get("stages/" + name + ".lua")))
    {
        LOG(Error, "Error while loading stage.");
        return false;
    }

    LOG(Info, "Loaded stage:", name);
    return true;
}

void StageController::onUpdate(float delta)
{
    //Scroll the background
    background->setPosition(background->getLocalPosition2D() + background_speed * double(delta));

    //Update the camera position
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
    {
        sp::Vector2d position = position_accumulator / double(ship_count) * 0.2;
        if (stage_done)
        {
            stage_done_time += delta;
            background_speed.x -= stage_done_time;
            position.x -= stage_done_time * stage_done_time * 10.0;
            if (position.x < -30.0)
            {
                SceneManager::instance->switchToStageSelect();

                for(int n=0; n<SaveData::instance->player_count; n++)
                {
                    SaveData::PlayerData& save_data = SaveData::instance->player_data[n];
                    if (player_data[n].ship)
                        save_data.hull = player_data[n].ship->hull->getHullLevel() / player_data[n].ship->hull->getMaxHullLevel();
                    else
                        save_data.hull = 0.0;
                }
            }
        }
        camera->setPosition(position);
    }
    
    //Update the message dialog
    message->hide();
    for(auto m : Message::messages)
    {
        if (m->isActive())
        {
            message->show();
            sp::P<sp::gui::Label> label = message->getWidgetWithID("TEXT");
            label->setAttribute("caption", m->getText());
            sp::P<sp::gui::Image> image = message->getWidgetWithID("FACE");
            sp::string face = m->getFace();
            if (face == "")
            {
                image->hide();
            }
            else
            {
                image->setAttribute("texture", face);
                image->show();
            }
            
            break;
        }
    }
    
    //Update the player HUD
    for(int n=0; n<SaveData::instance->player_count; n++)
    {
        updateHud(player_data[n]);
    }
    
    //Update the main hud
    if (SaveData::instance->pla < 1)
    {
        main_hud->getWidgetWithID("PLA")->hide();
    }
    else
    {
        main_hud->getWidgetWithID("PLA")->show();
        main_hud->getWidgetWithID("PLA_LABEL")->setAttribute("caption", sp::string(SaveData::instance->pla));
    }
    
    if (ship_count > 0)
    {
        main_hud->getWidgetWithID("GAMEOVER")->hide();
        game_over_delay = 3.0;
    }
    else
    {
        main_hud->getWidgetWithID("GAMEOVER")->show();
        game_over_delay -= delta;
        if (game_over_delay <= 0.0)
        {
            SceneManager::instance->switchToTitle();
        }
    }
    
    sp::P<sp::gui::Progressbar> boss = main_hud->getWidgetWithID("BOSS");
    if (Enemy::bosses.size() > 0)
    {
        float health = 0.0;
        for(Enemy* e : Enemy::bosses)
            health += e->getHealth();
        boss->show();
        boss->setValue(health);
        if (health > boss->getMaxRange())
            boss->setRange(0, health);
    }
    else
    {
        boss->hide();
        boss->setRange(0, 0);
    }
}

void StageController::onFixedUpdate()
{
    global_time++;
    script->call("update");
}

void StageController::updateHud(PlayerData& data)
{
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
