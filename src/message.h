#ifndef MESSAGE_H
#define MESSAGE_H

#include <sp2/scene/node.h>

class Message : public sp::Node
{
public:
    Message();
    ~Message();

    void setText(sp::string text);
    void setFace(sp::string face);
    void show();
    bool isActive() { return is_active; }
    sp::string getText();
    sp::string getFace();
    
    static sp::PList<Message> messages;
private:
    static constexpr float characters_per_second = 15.0f;
    static constexpr float extra_delay_before_done = 2.5f;

    sp::string text;
    sp::string face;
    float time;
    bool is_active;
    
    sp::script::Callback onFinished;
     
    virtual void onUpdate(float delta) override;
    virtual void onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class) override;
};

#endif//MESSAGE_H
