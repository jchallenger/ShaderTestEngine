#include "Sound.h"

Sound::Sound(std::string filename){
   position.x = 0;
   position.y = 0;
   position.z = 0;
}
void Sound::toggle(){
    bool paused;
    ch->getPaused(&paused);
    ch->setPaused(!paused);
}
void Sound::setPosition(float x, float y, float z){
    position.x = x;
    position.y = y;
    position.z = z;
    ch->set3DAttributes(&position, 0);
}
SoundManager::SoundManager(void)
{
    FMOD_RESULT errCheck;
    unsigned int version;

    globalCh = 0;

    errCheck = FMOD::System_Create(&system);
    ERR_Check(errCheck);

    errCheck = system->getVersion(&version);
    ERR_Check(errCheck);
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    }

    /* Init */
    errCheck = system->init(100, FMOD_INIT_NORMAL, 0);
    if (errCheck == FMOD_ERR_OUTPUT_CREATEBUFFER)         
    {
        /* Speaker mode selected isn't supported by this soundcard.
           Switch it back to stereo */
        errCheck = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERR_Check(errCheck);
        
        /* Re-Init */
        errCheck = system->init(100, FMOD_INIT_NORMAL, 0);
        ERR_Check(errCheck);
    }

    /* Display Hardware Channels*/
    /*{
        int numchannels;

        errCheck = system->getHardwareChannels(&numchannels);
        ERR_Check(errCheck);
    
        printf("Hardware channels : %d\n", numchannels);
    }*/

    system->set3DSettings(1.0, DISTANCE_FACTOR, 1.0f);

    listener = new Listener();
}
FMOD_MODE Sound::toggleMode(){
    FMOD_MODE mode;
    sound->getMode(&mode);

    if(mode & FMOD_3D_LINEARROLLOFF ){
        sound->setMode(FMOD_3D_LOGROLLOFF);
    }
    else{
        sound->setMode(FMOD_3D_LINEARROLLOFF);
    }
    system->playSound(FMOD_CHANNEL_REUSE, sound, false, &ch);

    sound->getMode(&mode);
    return mode;
}

SoundManager::~SoundManager(void)
{
    std::vector<Sound *>::iterator it;
    for(it = soundList.begin(); it != soundList.end(); it++){
        errCheck = (*it)->sound->release();
        ERR_Check(errCheck);
    }

    errCheck = system->close();
    ERR_Check(errCheck);
    errCheck = system->release();
    ERR_Check(errCheck);
}

int SoundManager::ERR_Check(FMOD_RESULT result){
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return result;
    }
    return 0;
}

Sound * SoundManager::addSound(std::string filename){
    Sound * newSound = new Sound(filename);
    // System Create
    errCheck = system->createStream(filename.c_str(), FMOD_3D, 0, &newSound->sound);
    ERR_Check(errCheck);

    // Sound Atributes
    errCheck = newSound->sound->set3DMinMaxDistance(10.0f * DISTANCE_FACTOR, 95.0f * DISTANCE_FACTOR);
    ERR_Check(errCheck);
    errCheck = newSound->sound->setMode(FMOD_LOOP_NORMAL);
    ERR_Check(errCheck);

    // Play Sound on Channel
    errCheck = system->playSound(FMOD_CHANNEL_FREE, newSound->sound, false, &globalCh);
    ERR_Check(errCheck);

    //ChannelAttributes
    errCheck = globalCh->set3DAttributes(&newSound->position, 0);
    ERR_Check(errCheck);

    newSound->ch = globalCh;
    newSound->system = system;

    soundList.push_back(newSound);

    return newSound;
}

void SoundManager::update(float delta){
    errCheck = system->set3DListenerAttributes(0, &listener->Position, 0, &listener->Direction, &listener->Up);
    ERR_Check(errCheck);

    system->update();
}