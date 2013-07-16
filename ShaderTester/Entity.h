#pragma once
#include "VectorOperations.h"
#include "Quaternion.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Scene;

/*
 Justin Challenger
 Entity
 **********************************************************************
 Created:   November 28th 2012

 Description:
 An Entity class that will hold several models, and traverse through them 
 to create animations

 Dependencies:

 Model class
     - Texture Util
     - Shader Util
 Vector Class
 Quaternion Class
 Scene Class (Scene)

 **********************************************************************
 Updated:   February 2, 2013
*/

enum DrawMode{
    IMMEDIATE = 0,
    RETAINED
};
enum texType{
    DIF_MAP = 0,
    SPHERE_MAP,
    CUBE_MAP
};
struct _AnimTime{
    // Time between 0 and the animation length in seconds
    float currAnimTime, duration;
    // Time since the beginning of the Entity's life
    float totalTime;
    // Next keyframe to switch to
    int nextAnim;
    // Repeat/Stop/Start bools
    bool cycleDone, cycleStart, cycleRepeat;
};

class Entity
{
public:
    Entity(void);
    // Addmodel from this filename
    Entity(std::string filename, Shader * sh);   
    ~Entity(void);
    void init();

    // Update Entity
    void update(float delta);
    // Draw current model
    void draw();
    // Set Shader for all Models
    void setShader(Shader * newShader);
    void setScene(Scene * _scene);

    void setPosition(Vec3 newPos);
    void setPosition(float x, float y, float z);
    void setStartPosition(Vec3 newStart);
    void setStartPosition(float x, float y, float z);
    void setScale(float size);
    void setScale(Vec3 newscale);
    void setScale(float x, float y, float z);

    void setModelMvp(glm::mat4 & m, glm::mat4 & v, glm::mat4 & p);

    void addModel(std::string filename);

    _AnimTime  * getAnimTimer();
    Vec3 getStartPosition();
    Vec3 getPosition();
    Vec3 getDirection();
    Vec3 getRight();
    Vec3 getUp();

    Model * currentModel;
    Model * targetMorph;
    std::vector<Model *> animList;

    float xRot, yRot, zRot;

    
    DrawMode drawMode;
    GLuint texture, normalMap, displaceMap;
    short textureType;

protected:
    Shader * shader;                // Points to scene Shader
    _AnimTime animTimer;            // Animation Timer
    
    Scene * scene;              // Points to parent Scene

    // World Transforms
    Vec3 start_Position, position, scale, direction, right, up;
    Quaternion orientation;
};

