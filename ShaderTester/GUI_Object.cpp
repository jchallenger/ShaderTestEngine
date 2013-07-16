#include "GUI_Object.h"
#include "GUI.h"

GUI_Object::GUI_Object(void)
{
    reset();
}

GUI_Object::~GUI_Object(void)
{
}

void GUI_Object::reset(){
    setMinColour(0.5f,0.5f,0.5f,1.f);
    setMaxColour(1,1,1,1);
    setClicked(false);
    setHovered(false);
    setHeld(false);

    setSize(100, 30);
    setPosition(0,0);
    hoverTime = 0; holdTime = 0; clickCount = 0;
}

void GUI_Object::setColour(     float _r, float _g, float _b, float _a){
    r       =_r; g      =_g; b      =_b; a      =_a;
}
void GUI_Object::setMinColour(  float _r, float _g, float _b, float _a){
    r_Min   =_r; g_Min  =_g; b_Min  =_b; a_Min  =_a;
}
void GUI_Object::setMaxColour(  float _r, float _g, float _b, float _a){
    r_Max   =_r; g_Max  =_g; b_Max  =_b; a_Max  =_a;
}
void GUI_Object::setFixedColour(float _r, float _g, float _b, float _a){
    setColour(      _r,_g,_b,_a);
    setMinColour(   _r,_g,_b,_a);
    setMaxColour(   _r,_g,_b,_a);
    texture = TextureUtils::loadTexture("Resources/Images/Blank.png");
}
void GUI_Object::setTexture(std::string filename){
    texture = TextureUtils::loadTexture(filename);
}

Vec3 GUI_Object::getPosition(){
    return position;
}
void GUI_Object::setPosition(float x, float y, float z){
    position.set(x,y,z);
}
void GUI_Object::setPosition(Vec3 & _position){
    position.set(_position);
}
Vec3 GUI_Object::getSize(){
    return size;
}
void GUI_Object::setSize(float x, float y, float z){
    size.set(x,y,z);
}
void GUI_Object::setSize(Vec3 & _size){
    size.set(_size);
}

void GUI_Object::update(float delta){
    // Check if Mouse is hovering element
    if( Input::getInstance()->getMousePos().x   > position.x            && 
        Input::getInstance()->getMousePos().x   < position.x+size.x     &&
        abs(Input::getInstance()->getMousePos().y - WindowInfo::getInstance()->getHeight())    > position.y         &&
        abs(Input::getInstance()->getMousePos().y - WindowInfo::getInstance()->getHeight())    < position.y+size.y  )
    {
        setHovered(true);
        hoverTime += delta;
    }
    else{
        setHovered(false);
        //hoverTime = 0;
    }
    
    if(!isHovering()){
        if(hoverTime > 1) { hoverTime = 1; }
        else if(hoverTime < 0) { hoverTime = 0; }
        else { hoverTime -= delta; }
    }
    r = (r_Min*(1.f-hoverTime) + (r_Max * 2.f * hoverTime));   
    g = (g_Min*(1.f-hoverTime) + (g_Max * 2.f * hoverTime));   
    b = (b_Min*(1.f-hoverTime) + (b_Max * 2.f * hoverTime));   
    a = (a_Min*(1.f-hoverTime) + (a_Max * 2.f * hoverTime));

    // If element was clicked
    if(isHovering() && Input::getInstance()->getMouse()->Left->wasClicked()){
        setClicked(true);
    }
    else{
        setClicked(false);
    }
    // If element was double clicked
    if(isHovering() && Input::getInstance()->getMouse()->Left->wasDoubleClicked()){
        
    }
    else{

    }
    // If Button is held down
    if(isHovering() && Input::getInstance()->getMouse()->Left->isDown()){
        setHeld(true);
        holdTime += delta;
    }
    else{
        setHeld(false);
        holdTime = 0;
    }
}
void GUI_Object::draw(){
    glPushMatrix();
    glLineWidth(1);
    glColor4f(r,g,b,a);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);        glVertex2f(position.x,            position.y);
        glTexCoord2f(1,1);        glVertex2f(position.x+size.x,     position.y);
        glTexCoord2f(1,0);        glVertex2f(position.x+size.x,     position.y + size.y);
        glTexCoord2f(0,0);        glVertex2f(position.x,            position.y + size.y);
    glEnd();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(1,1,1,1);
    glPopMatrix();
}
  
void GUI_Object::setClicked(bool set){
    clicked = set;
    clickCount++;
}
void GUI_Object::setHovered(bool set){
    hovered = set;
}
void GUI_Object::setHeld(bool set){
    held = set;
}
bool GUI_Object::isHeld(){
    return held;
}
bool GUI_Object::isHovering(){
    return hovered;
}
bool GUI_Object::wasClicked(){
    return clicked;
}