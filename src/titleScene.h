#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include <sp2/scene/node.h>
#include <sp2/graphics/gui/widget/widget.h>

class TitleController : public sp::Node
{
public:
    TitleController();
    
    void show();
    void hide();
    
    virtual void onUpdate(float delta) override;
private:
    void startNewGame(int player_count);
    
    static constexpr float text_scroll_speed = 5.0;
    static constexpr float text_scroll_start = -20.0;
    static constexpr float text_scroll_passed = 400.0;
    static constexpr float text_scroll_end = 650.0;

    enum class State
    {
        Scroller,
        GameSelect
    } state;
    float state_time;
    int option_index;
    sp::P<sp::Node> title;
    sp::P<sp::Node> text_scroll;
    sp::P<sp::gui::Widget> title_gui;
    sp::P<sp::gui::Widget> game_select_gui;
};

#endif//TITLE_SCENE_H
