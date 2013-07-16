#include "GUI.h"

/***************************************
**************** Button ****************
****************************************

Button::Button(){
    r=0; g=0; b=0; a=1.f;
    r_Min = 0.5;    g_Min = 0.5;    b_Min = 0.5;    a_Min = 0.5;
    r_Max = 1;      g_Max = 1;      b_Max = 1;      a_Max = 1;
    clicked = false;
    width = 100;
    height = 30;
    hoverTime = 0;
    texture = TextureUtils::loadTexture("Resources/Images/Blank.png");
}
Button::~Button(){

}
void Button::setColour(float _r, float _g, float _b, float _a){
    r       =_r; g      =_g; b      =_b; a      =_a;
    r_Min   =_r; g_Min  =_g; b_Min  =_b; a_Min  =_a;
    r_Max   =_r; g_Max  =_g; b_Max  =_b; a_Max  =_a;
}
void Button::setMinColour(float _r, float _g, float _b, float _a){
    r_Min   =_r; g_Min  =_g; b_Min  =_b; a_Min  =_a;
}
void Button::setMaxColour(float _r, float _g, float _b, float _a){
    r_Max   =_r; g_Max  =_g; b_Max  =_b; a_Max  =_a;
}
void Button::setTexture(std::string filename){
    texture = TextureUtils::loadTexture(filename);
}
void Button::draw(){
    glPushMatrix();
    glLineWidth(1);
    glColor4f(r,g,b,a);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);        glVertex2f(x,          y);
        glTexCoord2f(1,1);        glVertex2f(x+width,    y);
        glTexCoord2f(1,0);        glVertex2f(x+width,    y + height);
        glTexCoord2f(0,0);        glVertex2f(x,          y + height);
    glEnd();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f(1,1,1,1);
    glPopMatrix();
}
void Button::update(float delta){
    if(isHovering()){
        hoverTime += delta;   
    }
    else{
        
        if(hoverTime > 1) { hoverTime = 1; }
        else if(hoverTime < 0) { hoverTime = 0; }
        else { hoverTime -= delta; }
    }
    r = (r_Min*(1.f-hoverTime) + (r_Max * 2.f * hoverTime));   
    g = (g_Min*(1.f-hoverTime) + (g_Max * 2.f * hoverTime));   
    b = (b_Min*(1.f-hoverTime) + (b_Max * 2.f * hoverTime));   
    a = (a_Min*(1.f-hoverTime) + (a_Max * 2.f * hoverTime));
}
bool Button::isHovering(){
    if(gui->isVisible()){
        if( Input::getInstance()->getMousePos().x             > x         && 
            Input::getInstance()->getMousePos().x             < x+width   &&
            abs(Input::getInstance()->getMousePos().y - WindowInfo::getInstance()->getHeight())    > y         &&
            abs(Input::getInstance()->getMousePos().y - WindowInfo::getInstance()->getHeight())    < y+height  )
            {
                return true;
            }
    }
    return false;
}
bool Button::isToggledOn() { 
    return clicked; 
}
void Button::toggle() { 
    clicked = !clicked; 
}

/***************************************
**************** Slider ****************
****************************************
Slider::Slider(){
    slider = new GUI_Toggle();
    slider->setHeight(15);
    slider->setWidth(10);
    gui = NULL;

    r = g = b = a = 1;

    x = 0;
    x_end = 100;
    y = 10;
    y_end = 0;

    minVal = 0;
    maxVal = 1;

    line_width = 10;
    currentVal = 0;
    
    texture = TextureUtils::loadTexture("Resources/Images/Blank.png");
}
void Slider::setColour(float _r, float _g, float _b, float _a){
    r=_r; g=_g; b=_b; a=_a;
}
void Slider::setRange(float min, float max){
    minVal = min;
    maxVal = max;
}
void Slider::update(float delta){
    slider->update(delta);

    if(Input::getInstance()->getMouse()->sfMouse->isButtonPressed(sf::Mouse::Button::Left)){
        if(slider->isHovering())
        {
            float mousePos = (Input::getInstance()->getMousePos().x);
            currentVal = (mousePos - x - slider->getWidth()/2)/x_end;
            if(     currentVal < 0) currentVal = 0;
            else if(currentVal > 1) currentVal = 1;
        }
    }
    slider->x = x + (currentVal * x_end);
    slider->y = y - (line_width/4);//((1-currentVal) * y + (currentVal * 2 * y_end));
}
void Slider::draw(){
    slider->draw();
    glPushMatrix();
    glLineWidth(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Top
    glColor4f(0,g,b,a);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);        glVertex2f(x,          y);
        glTexCoord2f(1,1);        glVertex2f(x+(currentVal*x_end),    y);
        glTexCoord2f(1,0);        glVertex2f(x+(currentVal*x_end),    y + line_width);
        glTexCoord2f(0,0);        glVertex2f(x,          y + line_width);
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
        glTexCoord2f(0,1);        glVertex2f(x,          y);
        glTexCoord2f(1,1);        glVertex2f(x+x_end,    y);
        glTexCoord2f(1,0);        glVertex2f(x+x_end,    y + line_width);
        glTexCoord2f(0,0);        glVertex2f(x,          y + line_width);
    glEnd();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

}
float Slider::getVal(){
    return minVal + (currentVal*maxVal);
    //return sqrt(((x_end-x_beg)*(x_end*x_beg))+((y_end-y_beg)*(y_end*y_beg)));
    //return 0;
}
float Slider::getWidth(){
    return x_end + line_width;
}
float Slider::getHeight(){
    return y_end + line_width;
}
void Slider::setBeg(float _x, float _y){
    x = _x; y = _y;
}
void Slider::setEnd(float _x, float _y){
    x_end = _x; y_end = _y;
}
/***************************************
******************GUI*******************
****************************************/
GUI::GUI(void)
{
    r=1; g=1; b=1; a=1.f;
    visible = false;
    numOfElements = 0;
    cushion = 5;
    
    height = cushion*2;
    width = 20;

    texture = TextureUtils::loadTexture("Resources/Images/Blank.png");
    uiShader    = new Shader("Resources/Shaders/Basic120");
}


GUI::~GUI(void)
{
}

void GUI::setColour(float _r, float _g, float _b, float _a){
    r=_r; g=_g; b=_b; a=_a;
}
void GUI::setPos(int _x,int _y){
    x = _x;
    y = _y;
    height = cushion*2;
    numOfElements = 0;

    std::map<std::string,GUI_Object *>::iterator itr;
    for(itr = elements.begin(); itr != elements.end(); itr++){
        height += itr->second->getSize().y;
        itr->second->setPosition(x + cushion, (y + cushion) - (height+(cushion*numOfElements)));
        numOfElements++;
    }
}
void GUI::toggleDisplay(){
    visible = !visible; 
}
//
//void GUI::add(Button * button){
//    buttons.push_back(button);
//    width = button->getWidth() + (cushion * 2);
//    height += button->getHeight();
//    button->gui = this;
//    button->x = x + cushion;
//    button->y = (y + cushion) - (height+(cushion*numOfButtons)+(cushion*numOfSliders) );
//    numOfButtons++;
//}
//void GUI::add(Slider * slider){
//    sliders.push_back(slider);
//    float wide = slider->getWidth() + cushion;
//    if(width < wide){
//        cushion += (wide-width) /2;
//        width = wide;
//    }
//    height += slider->line_width;
//    slider->gui = this;
//    slider->slider->gui = this;
//    slider->setBeg(x + cushion,(y + cushion) - (height+(cushion*numOfButtons)+(cushion*numOfSliders) ));
//    numOfSliders++;
//}

void GUI::add(std::string ID, GUI_Object * object){
    elements.insert(map<std::string, GUI_Object *>::value_type(ID, object));

    width = object->getSize().x + (cushion * 2);
    height += object->getSize().y;
    numOfElements++;
}
void GUI::draw(){
    if(visible){
        glPushMatrix();
        uiShader->use();
        //glUseProgram(0);
        glPolygonMode(GL_FRONT, GL_FILL);
        glDisable(GL_LIGHTING);

        // Setup a Orthographic projection
        glViewport(0, 0, WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0,WindowInfo::getInstance()->getWidth(),0, WindowInfo::getInstance()->getHeight());

        glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
        
        std::map<std::string,GUI_Object *>::iterator itr;
        for(itr = elements.begin(); itr != elements.end(); itr++){
            itr->second->draw();
        }
        
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2);
        glColor4f(r,g,b,a);

        glBegin(GL_QUADS);
            glTexCoord2f(0,1);  glVertex2f(x,       y);
            glTexCoord2f(1,1);  glVertex2f(x+width, y);
            glTexCoord2f(1,0);  glVertex2f(x+width, y-height-(cushion*(numOfElements-1)));
            glTexCoord2f(0,0);  glVertex2f(x,       y-height-(cushion*(numOfElements-1)));
        glEnd();
        glLineWidth(1);

        glPopMatrix();
    }
}
void GUI::update(float delta){
    if(isVisible()){
        std::map<std::string,GUI_Object *>::iterator itr;
        for(itr = elements.begin(); itr != elements.end(); itr++){
            itr->second->update(delta);
        }
    }
}