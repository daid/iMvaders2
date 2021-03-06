#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include <sp2/scene/node.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/gui/widget/widget.h>

class StageSelect : public sp::Node
{
public:
    StageSelect();
    
    void show();
    void hide();
    
    virtual void onUpdate(float delta) override;
private:
    void addStageButton(sp::P<sp::gui::Widget> row, int level, int sublevel);

    sp::P<sp::Node> title;
    sp::P<sp::gui::Widget> gui;
    
    int selection_level = 0;
    int selection_sublevel = 0;
    bool shop_selected = false;
    
    bool wait_for_it;
};

#endif//STAGE_SELECT_H
