#include "message.h"
#include "spacescene.h"
#include "keys.h"

sp::PList<Message> Message::messages;

Message::Message()
: sp::Node(space_scene->getRoot())
{
    is_active = false;
    
    messages.add(this);
}

Message::~Message()
{
}

sp::string Message::getText()
{
    unsigned int len = time * characters_per_second;
    if (len >= text.length())
        return text;
    return text.substr(0, len);
}

sp::string Message::getFace()
{
    if (face == "")
        return "";
    if (time >= float(text.length()) / characters_per_second)
        return "face/" + face + "_1.png";
    if (fmodf(time, 1.5 / characters_per_second * 2) >= 1.5 / characters_per_second)
        return "face/" + face + "_2.png";
    return "face/" + face + "_1.png";
}

void Message::setText(sp::string text)
{
    this->text = text.replace("|", "\n");
}

void Message::setFace(sp::string face)
{
    this->face = face;
}

void Message::show()
{
    LOG(Debug, "Showing:", text.replace("\n", "|"));
    is_active = true;
    time = 0.0;
}

void Message::onUpdate(float delta)
{
    if (!is_active)
        return;
    for (int n=0; n<max_players; n++)
    {
        if (player_keys[n]->skip.getDown() && time > 0.0)
        {
            if (time < float(text.length()) / characters_per_second)
                time = float(text.length()) / characters_per_second;
            else
                time += extra_delay_before_done;
        }
    }
    time += delta;
    if (time >= float(text.length()) / characters_per_second + extra_delay_before_done)
    {
        is_active = false;
        
        onFinished.call();
    }
}

void Message::onRegisterScriptBindings(sp::ScriptBindingClass& script_binding_class)
{
    sp::Node::onRegisterScriptBindings(script_binding_class);
    
    script_binding_class.bind("show", &Message::show);
    script_binding_class.bind("setText", &Message::setText);
    script_binding_class.bind("setFace", &Message::setFace);
    script_binding_class.bind("onFinished", onFinished);
}
