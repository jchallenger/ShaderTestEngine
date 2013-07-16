#pragma once
#include <string>
/*
 Justin Challenger
 Timer
 **********************************************************************
 Created:   February 3, 2013

 Description:
 A simple timer class, keeps track of total life time and current time.

 **********************************************************************
 Updated:   February 3, 2013
*/

class myTimer
{
public:
    myTimer(void);
    myTimer(std::string _name);
    ~myTimer(void);

    void update(float delta);
    
    void start(); 
    void reset();
    void pause();
    void unpause();

    void setLoop(bool loops);
    void setLifeTime(float lifeTime);

    float getCurrentTime();
    float getTotalLifeTime();
    
    bool isPaused();
    bool isStarted();
    bool isTicking();
    bool isExpired();

private:
    std::string name;
    float totalLifeTime;
    float currTime, prevDelta, expireTime;

    bool loop, started, paused, expired;
};

