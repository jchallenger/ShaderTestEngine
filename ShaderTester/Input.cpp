#include "Input.h"

Mouse::Mouse(){
    sfMouse = new sf::Mouse();
    Left    = new MouseButton();
    Right   = new MouseButton();
    Middle  = new MouseButton();
}
Mouse::~Mouse(){

}
void Mouse::update(float delta){
    // Set Buttons
    Left->down      = sfMouse->isButtonPressed(sf::Mouse::Left);
    Right->down     = sfMouse->isButtonPressed(sf::Mouse::Right);
    Middle->down    = sfMouse->isButtonPressed(sf::Mouse::Middle);

    Left->update(delta);
    Right->update(delta);
    Middle->update(delta);
}
MouseButton::MouseButton(){
    clickTimeGo     = false;
    down            = false;
    clicked         = false;
    doubleClicked   = false;
    holdTime        = 0.0f;
    clickTime       = 0.0f;
    count           = 0;
    totalClicks     = 0;
}
void MouseButton::update(float delta){
    clicked = false;
    doubleClicked = false;

    if(down){
        holdTime += delta;
    }
    else{
        if(holdTime < CLICK_TIME && holdTime > 0.0f){
            clicked = true;
            clickTimeGo = true;
            count++;
            totalClicks++;
        }
        holdTime = 0;
    }
    if(clickTimeGo){
        clickTime += delta;
        if(clickTime > CLICK_TIME){
            clicked = false;
            clickTime = 0;
            clickTimeGo = false;
            count = 0;
        }
        if(clicked && count >=2){
            doubleClicked = true;
        }
        else{
            doubleClicked = false;
        }
    }
}
bool MouseButton::wasClicked(){
    return clicked;
    /*if(clicked){
        count = 0;
        clicked = false;
        return true;
    }
    return false;*/
}
bool MouseButton::wasDoubleClicked(){
    /*if(doubleClicked){
        count = 0;
        clicked = false;
        doubleClicked = false;
        return true;
    }
    return false;*/
    return doubleClicked;
}
bool MouseButton::isHeld(){
    if(holdTime > CLICK_TIME){
        return true;
    }
    return false;
}
bool MouseButton::isDown(){
    return down;
}

Input::Input(void)
{

}

Input::~Input(void)
{
}

void Input::update(float delta){
    Mouse::update(delta);
}

void Input::setMousePos(int x, int y, bool absolute){
    if(absolute){
        sfMouse->setPosition(sf::Vector2i(x,y));
    }
    sfMouse->setPosition(sf::Vector2i(x,y), *WindowInfo::getInstance()->getWindow());
}
sf::Vector2i Input::getMousePos(bool absolute){
    if(absolute){
        return sfMouse->getPosition();
    }
    return sfMouse->getPosition(*WindowInfo::getInstance()->getWindow());
}