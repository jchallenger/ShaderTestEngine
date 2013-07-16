#include "DisplacementMappingScene.h"


DisplacementMappingScene::DisplacementMappingScene(void) : Scene()
{
}

DisplacementMappingScene::~DisplacementMappingScene(void)
{
}

bool DisplacementMappingScene::init()
{
    Scene::init();
    gui = new GUI();
    gui->setPos(300,400);

    lightSwitch    = new GUI_Toggle();
    lightSwitch->setTexture("Resources/Images/sky.jpg");
    lightSwitch->setMaxColour(1,1,0,1);
    lightSwitch->setMinColour(0.5,0.5,0,0.5);

    drawModeSwitch = new GUI_Toggle();
    drawModeSwitch->setColour(0,0.4,1,0.7);

    wireSwitch     = new GUI_Toggle();
    textureSwitch  = new GUI_Toggle();
    exitSwitch     = new GUI_Toggle();
    exitSwitch->setColour(1.0f,0.0f,0.0f,0.7f);

    someSlider     = new GUI_Slider();     // Test Slider

    gui->add("Light",       lightSwitch    );
    gui->add("Draw Mode",   drawModeSwitch );
    gui->add("Wireframe",   wireSwitch     );
    gui->add("Textures",    textureSwitch  );
    gui->add("Exit",        exitSwitch     );
    gui->add("Slider1",     someSlider     );

    /* Set custom scene Shader */
    //phongShader = new Shader("Resources/Shaders/BasicTex");
    phongShader = new Shader("Resources/Shaders/sinDisplace", "Resources/Shaders/Basic");
    phongShader->setName("Light and Morph");

    basicMorphNoLights = new Shader("Resources/Shaders/Toon");
    basicMorphNoLights->setName("Morph");
    
    displacementShader = new Shader("Resources/Shaders/Displace", "Resources/Shaders/TNormal");
    displacementShader->setName("DisplaceMap");

    currentShader = displacementShader;
    
    /* Camera Settings */
    topCam->mBackward(0.5);
    rightCam->mBackward(0.5);
    frontCam->SetPosition(Vec3(0,0,0));

    perspCam->SetPosition(Vec3(50, 50, -50));
    perspCam->SetFocus(Vec3(0, 0, 0));
    perspCam->wireFrame = false;

    /* Sound Assignment */
    sound1_Assignment_RotateClock = new myTimer("Rotation Clock");
    sound1_Assignment_RotateClock->start();
    sound1_Assignment_RotateClock->pause();
    sound1_Assignment_TransClock = new myTimer("Translate Clock");
    sound1_Assignment_TransClock->start();
    sound1_Assignment_TransClock->pause();
    
    /* Entities */
    Entity * temp;

    temp = new Entity("Resources/Models/HighPolyPlane_40k.obj", currentShader);
    temp->getAnimTimer()->duration = M_PI * 2;
    //temp->addModel("Resources/Models/morphTest2_model.obj");
    //temp->addModel("Resources/Models/morphTest3_model.obj");
    //temp->texture = TextureUtils::loadCubeEnvMap("Resources/Images/mars");
    temp->texture = TextureUtils::loadTexture("Resources/Images/sky.jpg");
    temp->displaceMap = TextureUtils::loadTexture("Resources/Images/Plane_Displ.png");
    temp->normalMap = TextureUtils::loadTexture("Resources/Images/Plane_T_Norm.png");

    temp->setStartPosition(0,0,0);
    temp->setPosition(0,0,0);
    temp->textureType = DIF_MAP;
    temp->setScene(this);
    listOfEntities.push_back(temp);

    /* Set Player */
    currPlayer = temp;

    rDown = false;
    overlay = TextureUtils::loadTexture("Resources/Images/overlay.png");
    
    /* Sound */
    std::string soundName;
    //std::cout<<"\n\n******************************************************************************";
    //std::cout<<"\nPlace a sound in the '../Resources/Sounds' folder, and type the name here: \n";
    //std::cin >> soundName;
    soundName = "wave.mp3";
    sceneSound = sounds->addSound("Resources/Sounds/"+soundName);
    
    perspCam->speed = 20;
    frontCam->speed = 20;
    //Lights
    light[0] = new Light();
    light[0]->setBackup();
    light[0]->setShader(currentShader);
    light[0]->setDif(1, 1, 1);
    light[0]->enable();

    light[1] = new Light();
    light[1]->setBackup();
    light[1]->setShader(currentShader);
    light[1]->setDif(1, 0, 0);
    light[1]->enable();

    light[2] = new Light();
    light[2]->setBackup();
    light[2]->setShader(currentShader);
    light[2]->setDif(0, 1, 0);
    light[2]->enable();

    light[3] = new Light();
    light[3]->setBackup();
    light[3]->setShader(currentShader);
    light[3]->setDif(0, 0, 1);
    light[3]->enable();

    return 1;
}
void DisplacementMappingScene::update(float delta){
    Scene::update(delta);
//*****************************************************************************
    {
    // Rotate camera
    if(Input::getInstance()->getMouse()->Left->isHeld() && !gui->isVisible()){
        if(WindowInfo::getInstance()->isMouseHidden()){
            selectedCam->handleMouseMove(Input::getInstance()->getMousePos().x,Input::getInstance()->getMousePos().y,WindowInfo::getInstance()->getWidth(),WindowInfo::getInstance()->getHeight());
        }
        WindowInfo::getInstance()->hideMouse();
        Input::getInstance()->setMousePos(WindowInfo::getInstance()->getCenterX(), WindowInfo::getInstance()->getCenterY());
    }
    else if(!Input::getInstance()->getMouse()->Left->isDown()){
        WindowInfo::getInstance()->showMouse();
    }

    // Select camera
    if(Input::getInstance()->getMouse()->Left->wasClicked()){
        gui->hide();
        if(!Input::getInstance()->getMouse()->Left->isHeld()){
            if(!WindowInfo::getInstance()->isFullScreen()){
                if(Input::getInstance()->getMousePos().x < WindowInfo::getInstance()->getCenterX()){
                    if(Input::getInstance()->getMousePos().y < WindowInfo::getInstance()->getCenterY()){
                        selectedCam = topCam;
                    }
                    else{
                        selectedCam = frontCam;
                    }
                }
                else{
                    if(Input::getInstance()->getMousePos().y < WindowInfo::getInstance()->getCenterY()){
                        selectedCam = rightCam;
                    }
                    else{
                        selectedCam = perspCam;
                    }
                }
            }
        }
    }
    // Set Fullscreen
    if(Input::getInstance()->getMouse()->Left->wasDoubleClicked()){
        WindowInfo::getInstance()->toggleFullScreen();
    }

    // Move
    if(/*Input::getInstance()->isKeyPressed(sf::Keyboard::W) || */Input::getInstance()->isKeyPressed(sf::Keyboard::Up)){
        selectedCam->mForward(delta);
    }
    if(/*Input::getInstance()->isKeyPressed(sf::Keyboard::S) || */Input::getInstance()->isKeyPressed(sf::Keyboard::Down)){
        selectedCam->mBackward(delta);
    }
    if(/*Input::getInstance()->isKeyPressed(sf::Keyboard::A) || */Input::getInstance()->isKeyPressed(sf::Keyboard::Left)){
        selectedCam->mLeft(delta);
    }
    if(/*Input::getInstance()->isKeyPressed(sf::Keyboard::D) || */Input::getInstance()->isKeyPressed(sf::Keyboard::Right)){
        selectedCam->mRight(delta);
    }
    if(Input::getInstance()->isKeyPressed(sf::Keyboard::RShift) || 
        Input::getInstance()->isKeyPressed(sf::Keyboard::LShift)){
            if(Input::getInstance()->isKeyPressed(sf::Keyboard::R)){
                selectedCam->yRot = 0;
                selectedCam->xRot = 0;
                //Stuff->selectedCam->reset();
            }
    }

    
    // MENU
    //*****************************************************************************
    gui->update(delta);
    if(Input::getInstance()->getMouse()->Right->wasClicked()){
        gui->setPos(Input::getInstance()->getMousePos().x,abs(Input::getInstance()->getMousePos().y-WindowInfo::getInstance()->getHeight()));
        gui->unHide();
    }
    // Shader Toggle
    if(lightSwitch->wasClicked()){
        selectedCam->LightsOn  = lightSwitch->getToggle();
        if(selectedCam->LightsOn){
            currentShader = phongShader;
        }
        else{
            currentShader = basicMorphNoLights;
        }
        this->setAllEntitiesShader(currentShader);
        for(int L = 0; L < Light::numOfLights; L++)
            light[L]->setShader(currentShader);
    }
    if(drawModeSwitch->isHovering()){
        if(drawMode == IMMEDIATE){
            drawMode = RETAINED;
        }
        else if(drawMode == RETAINED){
            drawMode = IMMEDIATE;
        }
                    
        std::vector<Entity *>::iterator it;
        for(it = listOfEntities.begin(); it != listOfEntities.end(); it++){
            Entity * sh = (*it);
            sh->setScene(this);
        }
    }
    selectedCam->TextureOn = textureSwitch->getToggle();
    selectedCam->wireFrame = wireSwitch->getToggle();
    // Exit
    if(exitSwitch->wasClicked()){
        WindowInfo::getInstance()->close();
    }

    //*****************************************************************************
    sound1_Assignment_RotateClock->update(delta);
    sound1_Assignment_TransClock->update(delta);

    //Keys
    {
        if(Input::getInstance()->isKeyPressed(sf::Keyboard::R)){
            rDown = true;
        }
        if(!Input::getInstance()->isKeyPressed(sf::Keyboard::R)){
            if(rDown == true){
                sceneSound->toggleMode();
                rDown = false;
            }
        }
        /* I should attach sounds to entities... */
        if(Input::getInstance()->isKeyPressed(sf::Keyboard::D)){
            sound1_Assignment_TransClock->unpause();
            currPlayer->setPosition(currPlayer->getStartPosition()*(sin(1-sound1_Assignment_TransClock->getCurrentTime())) );
        }
        if(!Input::getInstance()->isKeyPressed(sf::Keyboard::D)){
            sound1_Assignment_TransClock->pause();
        }
        if(Input::getInstance()->isKeyPressed(sf::Keyboard::Z)){
            sound1_Assignment_RotateClock->reset();
            sound1_Assignment_RotateClock->start();
            sound1_Assignment_TransClock->reset();
            sound1_Assignment_TransClock->start();
            currPlayer->setPosition(currPlayer->getStartPosition());
        }
        if(Input::getInstance()->isKeyPressed(sf::Keyboard::P)){
            sound1_Assignment_RotateClock->unpause();
        }
        if(!Input::getInstance()->isKeyPressed(sf::Keyboard::P)){
            sound1_Assignment_RotateClock->pause();
        }
    }

    currPlayer->setPosition( 
            currPlayer->getPosition().length() * sin(sound1_Assignment_RotateClock->getCurrentTime()),
            0,
            currPlayer->getPosition().length() * cos(sound1_Assignment_RotateClock->getCurrentTime()) );
    sceneSound->setPosition(
        currPlayer->getPosition().x,
        currPlayer->getPosition().y,
        currPlayer->getPosition().z);

    light[0]->setPos(perspCam->position);
    light[1]->setPos(currPlayer->getPosition());
    light[2]->setPos(Vec3(sin(sceneClock->getCurrentTime())*10, -2, sin(sceneClock->getCurrentTime()/2)*10));
    light[3]->setPos(Vec3(sin(sceneClock->getCurrentTime()*1.3)*10, -2, sin(sceneClock->getCurrentTime())*20));
    //light[0]->setPos(currPlayer->getPosition());
    }
}

void DisplacementMappingScene::draw(){

    for(int L = 0; L < Light::numOfLights; L++)
        light[L]->render();

    if(WindowInfo::getInstance()->isFullScreen()){
        glViewport(0, 0, WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
        currCamera = selectedCam;
        if(phongShader->uni.eyePosition != -1){
            glUniform3f(phongShader->uni.eyePosition, currCamera->position.x,currCamera->position.y,currCamera->position.z);
        }
        Scene::draw();
    }
    else{
        glViewport(
            0, 
            WindowInfo::getInstance()->getCenterY(), 
            WindowInfo::getInstance()->getCenterX(), 
            WindowInfo::getInstance()->getCenterY());
        currCamera = topCam;
        if(phongShader->uni.eyePosition != -1){
            glUniform3f(phongShader->uni.eyePosition, currCamera->position.x,currCamera->position.y,currCamera->position.z);
        }
        Scene::draw();
        

        glViewport(
            WindowInfo::getInstance()->getCenterX(), 
            WindowInfo::getInstance()->getCenterY(), 
            WindowInfo::getInstance()->getCenterX(), 
            WindowInfo::getInstance()->getCenterY());
        currCamera = rightCam;
        if(phongShader->uni.eyePosition != -1){
            glUniform3f(phongShader->uni.eyePosition, currCamera->position.x,currCamera->position.y,currCamera->position.z);
        }
        Scene::draw();
        

        glViewport(
            0, 
            0, 
            WindowInfo::getInstance()->getCenterX(), 
            WindowInfo::getInstance()->getCenterY());
        currCamera = frontCam;
        if(phongShader->uni.eyePosition != -1){
            glUniform3f(phongShader->uni.eyePosition, currCamera->position.x,currCamera->position.y,currCamera->position.z);
        }
        Scene::draw();

        glViewport(
            WindowInfo::getInstance()->getCenterX(), 
            0, 
            WindowInfo::getInstance()->getCenterX(), 
            WindowInfo::getInstance()->getCenterY());
        currCamera = perspCam;
        if(phongShader->uni.eyePosition != -1){
            glUniform3f(phongShader->uni.eyePosition, currCamera->position.x,currCamera->position.y,currCamera->position.z);
        }
        Scene::draw();
                
        glDisable(GL_DEPTH_TEST);

        glViewport(0, 0, WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
        //drawOverlay();
        drawBorders();
        glEnable(GL_DEPTH_TEST);
        
    }
    gui->draw();
}
void DisplacementMappingScene::drawBorders(){
    
    glUseProgram(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);

    glPushMatrix();
        // Setup a Orthographic projection
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0,1,0,1);
        glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor3f(0.6,0,0);
        glLineWidth(20);

        glBegin(GL_LINES);
            glVertex2f(0.5,0);
            glVertex2f(0.5,1);
        glEnd();

        glBegin(GL_LINES);
            glVertex2f(0,0.5);
            glVertex2f(1,0.5);
        glEnd();
    glPopMatrix();
    
    glLineWidth(1);
}
void DisplacementMappingScene::drawOverlay(){
    glUseProgram(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);

    // Setup a Orthographic projection
	glPushMatrix();
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0,1,1,0);
        glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
     
        //glEnable(GL_BLEND);
        glColor4f(1,1,1,1);

        //glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, overlay);
   
        glBegin(GL_QUADS);
            glNormal3f(0,0,-1);
            glVertex2f(0,0);    glTexCoord2f(0,1);
            glVertex2f(0,0);    glTexCoord2f(1,1);
            glVertex2f(1,1);    glTexCoord2f(1,0);
            glVertex2f(0,1);    glTexCoord2f(0,0);
        glEnd();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, 0);
}