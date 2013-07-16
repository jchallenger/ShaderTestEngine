#pragma once
#include "gui_object.h"
class GUI_Toggle :
    public GUI_Object
{
public:
    GUI_Toggle(void);
    ~GUI_Toggle(void);

    void update(float delta);

    bool getToggle();

private:
    bool toggle;
};

