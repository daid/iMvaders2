#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/directoryResourceProvider.h>
#include <sp2/graphics/gui/scene.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/textureManager.h>

#include "scenemanager.h"
#include "spacescene.h"
#include "shipTemplate.h"
#include "equipmentTemplate.h"
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
    sp::P<sp::Window> window = new sp::Window(4.0/3.0);
    sp::P<sp::Engine> engine = new sp::Engine();

    sp::SceneGraphicsLayer* scene_layer = new sp::SceneGraphicsLayer(1);
    scene_layer->addRenderPass(new sp::BasicNodeRenderPass());
#ifdef DEBUG
    scene_layer->addRenderPass(new sp::CollisionRenderPass());
#endif
    window->addLayer(scene_layer);
    
    //Initialize the player keys, needs to be done before keybindings are loaded.
    PlayerKeys::init();
    ShipTemplate::init();
    EquipmentTemplate::init();

    new SceneManager();
    engine->run();
    
    return 0;
}
