#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene(void)
{
}

bool Scene::init(){
    sceneClock = new myTimer("Scene Clock");
    sceneClock->start();

    sounds = new SoundManager();

    passThroughShader   = new Shader("Resources/Shaders/SolidColour");
    uiShader            = new Shader("Resources/Shaders/Basic120");

    /* Default Cameras */
    this->perspCam = new Camera();
    perspCam->SetPosition(Vec3(0,20,-20));
    perspCam->SetFocus(Vec3(0,0,0));
    listOfCameras.push_back(perspCam);
    this->topCam = new Camera();
    topCam->SetPosition(Vec3(0,50,-1));
    topCam->SetFocus(Vec3(0,0,0));
    listOfCameras.push_back(topCam);
    this->rightCam = new Camera();
    rightCam->SetPosition(Vec3(50,0,0));
    rightCam->SetFocus(Vec3(0,0,0));
    listOfCameras.push_back(rightCam);
    this->frontCam = new Camera();
    frontCam->SetPosition(Vec3(0,0,-50));
    frontCam->SetFocus(Vec3(0,0,0));
    listOfCameras.push_back(perspCam);

    currCamera = perspCam;
    selectedCam = perspCam;

    drawMode = RETAINED;

    return 1;
}
void Scene::setAllEntitiesShader(Shader * newShader){
    std::vector<Entity *>::iterator it;
    for(it = listOfEntities.begin(); it != listOfEntities.end(); it++){
        Entity * sh = (*it);
        sh->setShader(newShader);
    }
}
void Scene::update(float delta){
    sceneClock->update(delta);
    sounds->update(delta);

    std::vector<Entity *>::iterator it;
    for(it = listOfEntities.begin(); it != listOfEntities.end(); it++){
        Entity * sh = (*it);
        sh->update(delta);
    }
}
void Scene::draw(){
    //passThroughShader->use();
    /**********************************
    ********MATRIX CALCULATIONS********
    **********************************/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Camera Rotations
    glm::mat4 rot = 
        glm::rotate(glm::mat4(1.0f), currCamera->yRot, glm::vec3(0,1,0))*
        glm::rotate(glm::mat4(1.0f), currCamera->xRot, glm::vec3(currCamera->right.x,currCamera->right.y,currCamera->right.z));

    // View Matrix (Camera)
    m_veiw    = glm::lookAt(
        glm::vec3(
            currCamera->position.x,
            currCamera->position.y,
            currCamera->position.z),        // position (Eye)
        glm::vec3(
            currCamera->pointOfFocus.x,
            currCamera->pointOfFocus.y,
            currCamera->pointOfFocus.z),    // focus    (Center)
        glm::vec3(
            currCamera->up.x,
            currCamera->up.y,
            currCamera->up.z));             // up       (Up)

    // Projection Matrix
    m_proj    = glm::perspective(45.0f, WindowInfo::getInstance()->getWidth()/WindowInfo::getInstance()->getHeight(), 5.f, 300.0f);

    // Draw
    glPushMatrix();
        currCamera->draw();

        // Bind Matrices
        std::vector<Entity *>::iterator it;
        for(it = listOfEntities.begin(); it != listOfEntities.end(); it++){
            // Model Matrix 
            m_model   = glm::mat4(1.0f);
        
            Entity * dr = (*it);

            glPushMatrix();
                dr->setModelMvp(m_model, m_veiw, m_proj);
                dr->draw();
            glPopMatrix();
        }
    glPopMatrix();
}
