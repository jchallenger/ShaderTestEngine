#include "GUI_Toggle.h"


GUI_Toggle::GUI_Toggle(void): 
    GUI_Object()
{
    toggle = false;
}


GUI_Toggle::~GUI_Toggle(void)
{
}

void GUI_Toggle::update(float delta){
    GUI_Object::update(delta);
    if(wasClicked()){
        toggle = !toggle;
    }
}

bool GUI_Toggle::getToggle(){
    return toggle;
}
