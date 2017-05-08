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
#include <sp2/graphics/textureManager.h>

#include "scenemanager.h"
#include "spacescene.h"
#include "shiptemplate.h"
#include "equipmenttemplate.h"
#include "keys.h"
#include "playershipcontroller.h"
#include "aishipcontroller.h"

int main(int argc, char** argv)
{
    //Create resource providers, so we can load things.
    new sp::io::DirectoryResourceProvider("resources");
    
#ifndef DEBUG
    sp::textureManager.setFallbackColors(sp::Color::Transparent, sp::Color::Transparent);
#endif
    
    //Load our ui theme.
    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    
    //Create a window to render on, and our engine.
    new sp::Window(4.0/3.0);
    sp::P<sp::Engine> engine = new sp::Engine();
    
    //Initialize the player keys, needs to be done before keybindings are loaded.
    PlayerKeys::init();
    ShipTemplate::init();
    EquipmentTemplate::init();
    
    sp::SpriteManager::create("circle", "ship/circle.png", 1.0);
    sp::SpriteManager::create("logo", "logo.png", sp::Vector2f(60.0, 30));
    sp::SpriteManager::create("logo2", "logo2.png", sp::Vector2f(60.0, 60.0 / 512 * 135));
    sp::SpriteManager::create("scrolltext", "scrolltext.png", sp::Vector2f(71.6, 308));

    new SceneManager();
    engine->run();
    
    return 0;
}
