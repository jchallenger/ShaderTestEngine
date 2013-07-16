#pragma once
#include "TextureUtils.h"
#include "shader_Utils.h"
#include "Input.h"

#include "GUI_Button.h"
#include "GUI_Toggle.h"
#include "GUI_Slider.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <vector>
#include <map>

/*
class GUI;
class Button
{
public:
    Button();
    ~Button();
    
    void setColour(float _r, float _g, float _b, float _a);
    void setMinColour(float _r, float _g, float _b, float _a);
    void setMaxColour(float _r, float _g, float _b, float _a);

    void setTexture(std::string filename);
    int getWidth(){ return width; }
    int getHeight(){ return height; }

    void setWidth(int w){ width = w; }
    void setHeight(int h){ height = h; }

    bool isToggledOn();
    bool isHovering();
    bool wasClicked();

    void update(float delta);
    void toggle();
    void draw();
    
    GUI * gui;
    int x, y;

private:
    GLuint texture;
    float r, g, b, a;
    float r_Min, g_Min, b_Min, a_Min;
    float r_Max, g_Max, b_Max, a_Max;
    float hoverTime;
    int width, height;
    bool clicked;
};
class Toggle
{

};
class Slider
{
public:
    Slider();
    ~Slider();

    void setColour(float _r, float _g, float _b, float _a);
    
    void setBeg(float x, float y);
    void setEnd(float x, float y);

    float getWidth();
    float getHeight();
    
    void update(float delta);
    void draw();

    GUI * gui;
    Button * slider; 

    float x,y;
//private:
    GLuint texture;
    float currentVal;
    float minVal, maxVal;
    float line_width;
    float r, g, b, a;
    float x_end, y_end;
};
*/

class GUI_Object;
// GUI Class
class GUI
{
public:
    GUI(void);
    ~GUI(void);

    void setColour(float _r, float _g, float _b, float _a);
    void setPos(int,int);/*
    void add(Button * button);
    void add(Slider * slider);*/
    void add(std::string ID, GUI_Object * object);

    void draw();
    void update(float delta);

    void hide() {   visible = false;    }
    void unHide() { visible = true;     }
    bool isVisible() { return visible;  }
    void toggleDisplay();

    float getWidth();
    float getHeight();
/*
    std::vector<Button *> buttons;
    std::vector<Slider *> sliders;*/
    std::map<std::string, GUI_Object * > elements;
    
    int x, y;
    int /*numOfButtons, numOfSliders, */numOfElements, cushion;
    
    Shader * uiShader;
private:
    int height, width;
    float r, g, b, a;
    bool visible;

    GLuint texture;
};

