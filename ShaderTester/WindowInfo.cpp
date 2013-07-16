#include "WindowInfo.h"

WindowInfo::WindowInfo(){
    fullScreen  = false;
    mouseHide   = false;
}
WindowInfo::~WindowInfo()
{
}

void WindowInfo::setWindow(sf::RenderWindow * _window){
    window = _window;
    setSize( _window->getSize() );
}
sf::RenderWindow * WindowInfo::getWindow(){
    return window;
}

void WindowInfo::draw(){
    window->display();
}
void WindowInfo::close(){
    window->close();
}

void WindowInfo::setWidth(float x){
    window->setSize(sf::Vector2u(x,window->getSize().y));
    
    centerWidth = x/2;
}
void WindowInfo::setHeight(float y){
    window->setSize(sf::Vector2u(window->getSize().x,y));

    centerHeight = y/2;
}

void WindowInfo::setSize(unsigned int x, unsigned int y){
    setSize(sf::Vector2u(x,y));
}
void WindowInfo::setSize(sf::Vector2u _size){
    window->setSize(_size);

    centerHeight = _size.y/2;
    centerWidth = _size.x/2;
}

void WindowInfo::toggleFullScreen(){
    fullScreen = !fullScreen;
}

void WindowInfo::hideMouse(){
    mouseHide = true;
    window->setMouseCursorVisible(false);
}
void WindowInfo::showMouse(){
    mouseHide = false;
    window->setMouseCursorVisible(true);
}


bool WindowInfo::isFullScreen(){    return fullScreen;          }
bool WindowInfo::isMouseHidden(){   return mouseHide;           }

float WindowInfo::getWidth(){       return window->getSize().x; }
float WindowInfo::getHeight(){      return window->getSize().y; }
float WindowInfo::getCenterX(){     return centerWidth;         }
float WindowInfo::getCenterY(){     return centerHeight;        }