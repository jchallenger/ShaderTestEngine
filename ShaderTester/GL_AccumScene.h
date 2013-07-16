#include "Scene.h"


class TestScene1: public Scene
{
public:
    TestScene1(void);
    ~TestScene1(void);
    
    bool init();
    void update(float delta);
    void draw();

    bool rDown;
    myTimer * sound1_Assignment_RotateClock, * sound1_Assignment_TransClock;

    Shader * phongShader;
    Shader * basicMorphNoLights;
    Shader * manyLights;
    Shader * currentShader;

    Light * light[8];

    void drawBorders();
    void drawOverlay();
    GLuint overlay;

    GUI * gui;
    GUI_Toggle * drawModeSwitch;
    GUI_Toggle * lightSwitch;
    GUI_Toggle * wireSwitch; 
    GUI_Toggle * textureSwitch;
    GUI_Toggle * exitSwitch;   
    GUI_Slider * someSlider;
};

