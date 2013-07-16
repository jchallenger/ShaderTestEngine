/*
 Justin Challenger
 Main Driver
 **********************************************************************
 Created:   November 28th 2012

 Description:
 A temporary driver for my classes

 **********************************************************************
 Updated:   January 14th 2013
*/
#include "WindowInfo.h"
#include "TestScene.h"
#include "GL_AccumScene.h"
#include "NormalMapsScene.h"
#include "FBOPostProcessScene.h"
#include "DisplacementMappingScene.h"

#include "Emitter.h"
#include "Input.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

unsigned int Window_Width   = 800;
unsigned int Window_Height  = 600;
const std::string Window_Title = "Justin Tests";

void main(void){
    std::cout
        <<" =========================================== \n"
        <<"         ChallengerTestProgram 2013          \n"
        <<"             By Justin Challenger            \n"
        <<" =========================================== \n";

     /**********************************
    ***********CREATE WINDOW***********
    **********************************/
    
    sf::RenderWindow * App;

    sf::ContextSettings Settings;
    Settings.depthBits         = 24; // Request a 24 bits depth buffer
    Settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
    Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    

    App = new sf::RenderWindow(sf::VideoMode(
        Window_Width, Window_Height, 32), 
        Window_Title, 
        sf::Style::Close, 
        Settings);
    
    WindowInfo::getInstance()->setWindow(App);
    
	ilInit();
	iluInit();
    ilutInit();
	ilutRenderer(ILUT_OPENGL);

    /**********************************
    ************OpenGL Init************
    **********************************/

	glewInit();

    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);


    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);

    // Create a new Scene
    //Scene * Stuff = new TestScene1();        // MOTION BLUR 
    //Scene * Stuff = new NormalMapsScene();   // Normal Maps - Tangent - TOON
    Scene * Stuff = new FBOPostProcessScene();     // FBO not working
    //Scene * Stuff = new DisplacementMappingScene();  // Displacement Maps

    Emitter * emitt = new Emitter();
    emitt->createAllParticles();
    
    /**********************************
    *************GAME START************
    **********************************/
    if(Stuff->init() == 1)
    {
       // Init the Game Object
        sf::Clock updateClock;
        int nbFrames = 0;
        float delta = 0;

        bool isRunning = true, mouseDown = false, focused = true;
        double lastTime = 0, currentTime = 0, lastDraw = 0;

        /**********************************
        *************GAME LOOP*************
        **********************************/
        while (WindowInfo::getInstance()->getWindow()->isOpen())
        {
            // Measures Speed (How long it takes to draw a single frame in Milliseconds)
            currentTime += updateClock.getElapsedTime().asSeconds();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
                // printf and reset timer
                printf("\n%f ms/frame", 1000.0/double(nbFrames)); 
                nbFrames = 0;
                lastTime += 1.0;
            }

            delta = updateClock.restart().asSeconds();

            
            sf::Event Event;
            while (WindowInfo::getInstance()->getWindow()->pollEvent(Event))
            {
                // X Button on Window
                if(Event.type == sf::Event::Closed){ 
                    WindowInfo::getInstance()->getWindow()->close();
                }
                // Resize Window
                if (Event.type == sf::Event::Resized){
                    glViewport(0, 0, Event.size.width, Event.size.height);
                    WindowInfo::getInstance()->setHeight(  Event.size.height);
                    WindowInfo::getInstance()->setWidth(   Event.size.width);
                }
                
                if(Event.type == sf::Event::MouseWheelMoved){
                    // Zoom in and out
                    //if(Event.mouseWheel.delta == -1) TheGame->scenes.GetActiveScene()->activeCamera->zoom += 0.5;
                    //if(Event.mouseWheel.delta ==  1) TheGame->scenes.GetActiveScene()->activeCamera->zoom -= 0.5;
                }
                if(Event.type == sf::Event::GainedFocus){
                    focused = true;
                }
                if(Event.type == sf::Event::LostFocus){
                    focused = false;
                }
            }
            // If the window is in focus
            if(focused){
                // Update
                Input::getInstance()->update(delta);
                Stuff->update(delta);
                //emitt->update(delta);
            }
            // Clear and Draw
            Stuff->draw();

            // display on screen
            WindowInfo::getInstance()->draw();
           
        }

    }
    return;
}