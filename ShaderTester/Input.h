#pragma once
#include "Singleton.h"
#include "WindowInfo.h"

#define CLICK_TIME 0.25f
#define DBL_CLICK_INTRVL_TIME 0.15f

struct MouseButton{
    MouseButton();
    void update(float delta);
    bool isDown();
    bool isHeld();
    bool wasClicked();
    bool wasDoubleClicked();

    bool down, clicked, doubleClicked, clickTimeGo;
    float holdTime, clickTime;
    unsigned int count, totalClicks;
};

class Mouse
{
public:
    Mouse();
    ~Mouse();
    virtual void update(float delta);

    MouseButton * Left, * Right, * Middle;
    
//private:
    sf::Mouse * sfMouse;
};

class Input : public Singleton<Input>, public Mouse, public sf::Keyboard
{
public:
    Input(void);
    ~Input(void);

    void update(float delta);

    void setMousePos(int x, int y, bool absolute = false);
    sf::Vector2i getMousePos(bool absolute = false);

    Mouse * getMouse() { return this; };
    sf::Keyboard * getKeyboard() { return this; };

    //Mouse * mouse;
    //sf::Keyboard * keys;
};