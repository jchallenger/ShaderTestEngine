#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <string>
#include <vector>

//#include "Player.h"

class ObjectiveManager;

namespace ObjectiveType{
    enum Type{
        Count = 0,
        Time,
        Avoid,      // Undefined
        Special     // Undefined
    };
}
// Objective Types
class Objective{
public:
    Objective(std::string name);

    virtual void update(float delta) = 0;
    virtual void addProgress(float progress = 1.0f) = 0;
    virtual void setProgress(float progress) = 0;
    virtual void setLimit(float limit) = 0;
    
    virtual float getProgress() = 0;
    virtual float getLimit() = 0;

    void setID(int ID);
    void setName(std::string name);
    void setComplete(bool compl);

    std::string getName();
    int getID();
    bool isComplete();

    ObjectiveType::Type getType();
protected:
    float limit;
    bool complete;
    std::string objectiveName;
    unsigned int id;
    ObjectiveType::Type type;
};

/* Objective Manager */
class ObjectiveManager
{
public:
    ObjectiveManager();
    ~ObjectiveManager(void);

    void update(float delta);
    void addObjective(Objective * objective);
    void clear();
    bool allComplete();
    Objective * getObjective(std::string name);
    Objective * getObjective(unsigned int ID);

    //void setPlayer(Player *playerOne);
    //void setScene(Scene *scene);
    //Scene * getScene();
    //Player * getPlayer();

private:
    //Scene * currentScene;
    //Player * player;

    std::vector<Objective *> objList;
};

#endif