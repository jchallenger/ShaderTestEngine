#include "GUI_Slider.h"


GUI_Slider::GUI_Slider(void) : 
    GUI_Object()
{
    setSize(100, 10);
    slideButton = new GUI_Object();
    slideButton->setSize(10,15);
}


GUI_Slider::~GUI_Slider(void)
{
}


void GUI_Slider::draw(){
    slideButton->draw();
    glPushMatrix();
    glLineWidth(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Top
    glColor4f(0,g,b,a);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);        glVertex2f(position.x,                    position.y);
        glTexCoord2f(1,1);        glVertex2f(position.x+(currentVal*size.x),position.y);
        glTexCoord2f(1,0);        glVertex2f(position.x+(currentVal*size.x),position.y + size.y);
        glTexCoord2f(0,0);        glVertex2f(position.x,                    position.y + size.y);
    glEnd();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    // Underneath
    glColor4f(r,0,b,a);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);        glVertex2f(position.x,        position.y);
        glTexCoord2f(1,1);        glVertex2f(position.x+size.x, position.y);
        glTexCoord2f(1,0);        glVertex2f(position.x+size.x, position.y + size.y);
        glTexCoord2f(0,0);        glVertex2f(position.x,        position.y + size.y);
    glEnd();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void GUI_Slider::update(float delta){
    GUI_Object::update(delta);
    slideButton->update(delta);

    if(slideButton->isHeld())
    {
        float mousePos = (Input::getInstance()->getMousePos().x);
        currentVal = (mousePos - position.x - slideButton->getSize().x/2)/size.x;
        if(     currentVal < 0) currentVal = 0;
        else if(currentVal > 1) currentVal = 1;
    }
    slideButton->setPosition(position.x + (currentVal * size.x), position.y - (size.y/4));
}
    
void GUI_Slider::setRange(float min, float max){
    minVal = min;
    maxVal = max;
}
float GUI_Slider::getValue(){
    return minVal + (currentVal*maxVal);
}