#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/graphics/gui/graphicslayer.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/spriteManager.h>

#include "scenemanager.h"
#include "spacescene.h"
#include "shiptemplate.h"
#include "equipmenttemplate.h"
#include "keys.h"
#include "playershipcontroller.h"

int main(int argc, char** argv)
{
    //Create resource providers, so we can load things.
    new sp::io::DirectoryResourceProvider("resources");
    
    //Load our ui theme.
    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    
    //Create a window to render on, and our engine.
    new sp::Window();
    sp::P<sp::Engine> engine = new sp::Engine();
    
    //Initialize the player keys, needs to be done before keybindings are loaded.
    PlayerKeys::init();
    ShipTemplate::init();
    EquipmentTemplate::init();
    
    space_scene = new sp::Scene();

    sp::SpriteManager::create("circle", "ship/circle.png", 0.5);
    sp::SpriteManager::create("bullet", "weapon/bullet.png", 0.5);
    sp::SpriteManager::create("logo", "logo.png", sp::Vector2f(30.0, 15));
    sp::SpriteManager::create("logo2", "logo2.png", sp::Vector2f(30.0, 30.0 / 512 * 135));
    sp::SpriteManager::create("scrolltext", "scrolltext.png", sp::Vector2f(35.8, 154));

    for(int x=-10; x<10; x++)
    {
        for(int y=-10; y<10; y++)
        {
            sp::P<sp::SceneNode> node = new sp::SceneNode(space_scene->getRoot());
            node->setPosition(sp::Vector2d(x * 3, y * 3));
            node->render_data = sp::SpriteManager::get("circle");
        }
    }
    //space_scene->disable();
    
    new SceneManager();
        
    engine->run();
    
    return 0;
}
