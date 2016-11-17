#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include <sp2/scene/scene.h>
#include <sp2/graphics/scene/graphicslayer.h>

class TitleScene : public sp::Scene
{
public:
    TitleScene();
    
    virtual void onUpdate(float delta) override;
    
    void setTextScroller(bool enabled) { show_text_scroller = enabled; }
private:
    static constexpr float text_scroll_speed = 5.0;
    static constexpr float text_scroll_start = -175.0;
    static constexpr float text_scroll_end = 340.0;

    bool show_text_scroller;
    float offset;
    sp::P<sp::SceneNode> title;
    sp::P<sp::SceneNode> text_scroll;
    sp::P<sp::SceneGraphicsLayer> graphics_layer;
};

#endif//TITLE_SCENE_H
