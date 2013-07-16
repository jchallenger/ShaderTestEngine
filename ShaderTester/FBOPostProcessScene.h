#include "Scene.h"
#include <SFML/Graphics.hpp>

class FBOPostProcessScene: public Scene
{
public:
    FBOPostProcessScene(void);
    ~FBOPostProcessScene(void);
    
    bool init();
    void update(float delta);
    void draw();
    void drawFullScreenQuad();

    bool rDown;
    myTimer * sound1_Assignment_RotateClock, * sound1_Assignment_TransClock;

    Shader * phongShader;
    Shader * basicMorphNoLights;
    Shader * postProcess;
    Shader * currentShader;

    FBO * fbo1, * fbo2;
    sf::RenderTexture fboTextSFML;

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

