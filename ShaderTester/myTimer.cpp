#include "myTimer.h"


myTimer::myTimer(void)
{
    name = "";
    totalLifeTime = 0.0f;
    currTime = prevDelta = 0.0f; 
    expireTime = 0.0f;

    loop = started = paused = false;
}
myTimer::myTimer(std::string _name)
{
    name = _name;
    totalLifeTime = 0.0f;
    currTime = prevDelta = 0.0f; 
    expireTime = 0.0f;

    loop = started = paused = false;
}


myTimer::~myTimer(void)
{
}

void myTimer::update(float delta){
    prevDelta = delta;
    
    if(started){
        expired = false;
        totalLifeTime += delta;
        if(!paused){
            currTime += delta;
            if(currTime > expireTime && expireTime > 0.0f){
                expired = true;
                if(loop){
                    start();
                }
                else{
                    pause();
                }
            }
        }
    }
}
/* Resets and starts timer */
void myTimer::start(){
    currTime = 0;
    paused = false;
    started = true;
}
/* Resets and stops timer */
void myTimer::reset(){
    currTime = 0;
    totalLifeTime = 0;

    started = false;
    paused = false;
    expired = false;
}
/* Pauses timer, but keeps updating totalLifeTime */
void myTimer::pause(){
    paused = true;
}
/* Upauses timer */
void myTimer::unpause(){
    paused = false;
}
/* Sets repeatability */
void myTimer::setLoop(bool loops){
    loop = loops;
}
/* How long will the timer be active for? */
void myTimer::setLifeTime(float lifeTime){
    expireTime = lifeTime;
}


float myTimer::getCurrentTime(){
    return currTime;
}
float myTimer::getTotalLifeTime(){
    return totalLifeTime;
}

bool myTimer::isExpired(){
    return expired;
}
bool myTimer::isPaused(){
    return paused;
}
bool myTimer::isStarted(){
    return started;
}
bool myTimer::isTicking(){
    if(started && !paused){
        return true;
    }
    return false;
}