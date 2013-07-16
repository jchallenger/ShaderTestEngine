#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Singleton.h"

class WindowInfo : public Singleton<WindowInfo>
{
public:
    WindowInfo();
    ~WindowInfo();

    void setWindow(sf::RenderWindow *);
    sf::RenderWindow * getWindow();

    void draw();
    void close();

    void setWidth(float x);
    void setHeight(float y);
    void setSize(unsigned int x, unsigned int y);
    void setSize(sf::Vector2u _size);

    void toggleFullScreen();
    void hideMouse();
    void showMouse();

    float getWidth();
    float getHeight();
    float getCenterX();
    float getCenterY();

    bool isFullScreen();
    bool isMouseHidden();

private:
    float centerWidth, centerHeight;
    bool fullScreen, mouseHide;

    sf::RenderWindow * window;
};
#endif