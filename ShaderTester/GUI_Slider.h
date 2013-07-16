#pragma once
#include "gui_object.h"
class GUI_Slider :
    public GUI_Object
{
public:
    GUI_Slider(void);
    ~GUI_Slider(void);

    void draw();
    void update(float delta);
    
    void setRange(float min, float max);
    float getValue();

private:
    float minVal, maxVal, currentVal;
    GUI_Object * slideButton;
};

