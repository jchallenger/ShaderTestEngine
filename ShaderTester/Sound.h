#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>

#pragma once
/* 
 Feet           = 3.28f 
 Centimeters    = 100.0f
*/
#define DISTANCE_FACTOR 1.0f
struct Sound
{
    Sound(std::string);
    ~Sound();
    
    void toggle();
    FMOD_MODE toggleMode();
    void setPosition(float x, float y, float z);
    
    FMOD_VECTOR position;
    FMOD_VECTOR * rolloff;
    FMOD::Sound * sound;
    FMOD::System * system;
    FMOD::Channel * ch;
};
struct Listener{
    Listener(){
        Position.x = 0;
        Position.y = 0;
        Position.z = 0;
        
        Direction.x = 0;
        Direction.y = 0;
        Direction.z = -1;

        Right.x = 1;
        Right.y = 0;
        Right.z = 0;

        Up.x = 0;
        Up.y = 1;
        Up.z = 0;

    }
    FMOD_VECTOR Position, Up, Right, Direction, LastPos, Velocity;
};

class SoundManager
{
public:
    SoundManager(void);
    ~SoundManager(void);

    Sound * addSound(std::string filename);
    Sound * addSound(Sound &sound);

    void removeSound(std::string filename);
    void update(float delta);
    void init();
    int ERR_Check(FMOD_RESULT errCheck);

    Listener * listener;
    FMOD_SPEAKERMODE speakermode;

private:
    FMOD_RESULT errCheck;
    FMOD::System * system;
    FMOD::Channel * globalCh;
    std::vector<Sound *> soundList;
};

