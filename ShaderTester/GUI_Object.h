#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include "VectorOperations.h"
#include "TextureUtils.h"
#include "shader_Utils.h"
#include "Input.h"

class GUI_Object
{
public:
    GUI_Object(void);
    ~GUI_Object(void);

    void setColour(     float _r, float _g, float _b, float _a);
    void setFixedColour(float _r, float _g, float _b, float _a);
    void setMinColour(  float _r, float _g, float _b, float _a);
    void setMaxColour(  float _r, float _g, float _b, float _a);
    void setTexture(std::string filename);
    
    Vec3 getPosition();
    void setPosition(float x, float y, float z = 1);
    void setPosition(Vec3 & _position);
    Vec3 getSize();
    void setSize(float x, float y, float z = 1);
    void setSize(Vec3 & _size);

    virtual void update(float delta);
    virtual void draw();
    
    void setHeld(bool);
    void setHovered(bool);
    void setClicked(bool);

    bool isHeld();
    bool isHovering();
    bool wasClicked();
    
protected:
    void reset();

    unsigned int texture;

    Vec3 position;
    Vec3 size;
    float r, g, b, a;
    float r_Min, g_Min, b_Min, a_Min;
    float r_Max, g_Max, b_Max, a_Max;

    float hoverTime, holdTime;
    bool clicked, hovered, held;
    unsigned int clickCount;
};

#endif