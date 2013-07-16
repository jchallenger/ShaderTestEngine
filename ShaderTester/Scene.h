#pragma once
/*
 Justin Challenger
 Temporary Scene
 **********************************************************************
 Created:   November 28th 2012

 Description:
 This is a temporary class to hold my Entities, Emitters, and Cameras.
 This will act like a scene.

 **********************************************************************
 Updated:   Novemeber 28th 2012
*/

//#include <GL/glew.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "WindowInfo.h"
#include "Input.h"

#include "Sound.h"
#include "myTimer.h"
#include "Entity.h"
#include "Light.h"
#include "shader_Utils.h"
#include "Camera.h"
#include "GUI.h"

class Scene
{
public:
    Scene(void);
    ~Scene(void);

    virtual bool init();
    virtual void update(float delta);
    virtual void draw();
    void setAllEntitiesShader(Shader * newShader);

    Entity * currPlayer;
    std::vector<Entity *> listOfEntities;

    Camera * currCamera, *selectedCam, *topCam, *rightCam, *perspCam, *frontCam;
    std::vector<Camera* > listOfCameras;

    Shader * passThroughShader, * uiShader;

    myTimer * sceneClock;
    
    SoundManager * sounds;
    Sound * sceneSound;
    
    DrawMode drawMode;

    glm::mat4 m_model, m_proj, m_veiw;
};

