#include "titleScene.h"

#include <sp2/scene/cameraNode.h>
#include <sp2/graphics/spriteManager.h>
#include <sp2/graphics/shader.h>
#include <sp2/graphics/meshdata.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/logging.h>
#include <sp2/assert.h>

TitleScene::TitleScene()
{
    sp::P<sp::CameraNode> camera = new sp::CameraNode(getRoot());
    camera->setPerspective();
    camera->setPosition(sp::Vector3d(0, 0, 50));
    setDefaultCamera(camera);

    std::vector<sp::MeshData::Vertex> vertices;
    vertices.emplace_back(sf::Vector3f(-1, -1, 1), sp::Vector2f(0, 1));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f(-1,  1, 1), sp::Vector2f(0, 0));
    vertices.emplace_back(sf::Vector3f( 1, -1, 1), sp::Vector2f(1, 1));
    vertices.emplace_back(sf::Vector3f( 1,  1, 1), sp::Vector2f(1, 0));
    
    sp::P<sp::SceneNode> background = new sp::SceneNode(getRoot());
    background->render_data.type = sp::RenderData::Type::Normal;
    background->render_data.shader = sp::Shader::get("shader/background.shader");
    background->render_data.mesh = std::make_shared<sp::MeshData>(vertices);
    background->render_data.texture = "stars.png";
    background->render_data.color = sf::Color::White;
    
    title = new sp::SceneNode(getRoot());
    title->render_data = sp::SpriteManager::get("logo2");
    title->setPosition(sp::Vector2d(0, 15));

    sp::P<sp::SceneNode> text_scroll_root = new sp::SceneNode(getRoot());
    text_scroll_root->setPosition(sp::Vector2d(0, -15));
    text_scroll_root->setRotation(sp::Quaterniond::fromAxisAngle(sp::Vector3d(1, 0, 0), -80));

    offset = text_scroll_start;
    show_text_scroller = true;

    text_scroll = new sp::SceneNode(text_scroll_root);
    text_scroll->render_data = sp::SpriteManager::get("scrolltext");
    text_scroll->setPosition(sp::Vector2d(0, offset));
    
    //Build the renderer for this specific scene.
    graphics_layer = new sp::SceneGraphicsLayer(1);
    graphics_layer->addRenderPass(new sp::BasicNodeRenderPass("window", this));
}

void TitleScene::onUpdate(float delta)
{
    offset += delta * text_scroll_speed;
    if (offset > text_scroll_end || !show_text_scroller)
        offset = text_scroll_start;
    text_scroll->setPosition(sp::Vector2d(0, offset));
}
