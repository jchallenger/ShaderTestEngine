#include "ObjectiveManager.h"

/* Base Objective Abstract Class*/
Objective::Objective(std::string name){
    objectiveName = name;
    complete = false;
    type = ObjectiveType::Count;
}
void Objective::setName(std::string name){
    objectiveName = name;
}
void Objective::setID(int ID){
    id = ID;
}
std::string Objective::getName(){
    return objectiveName;
}
int Objective::getID(){
    return id;
}
ObjectiveType::Type Objective::getType(){
    return type;
}
bool Objective::isComplete(){
    return (getProgress() >= 1.0f);
}

/* Manager */
ObjectiveManager::ObjectiveManager()
{
}
ObjectiveManager::~ObjectiveManager(void)
{
}

void ObjectiveManager::update(float delta){
    std::vector<Objective *>::iterator it;
    for(it = objList.begin(); it != objList.end(); it++){
        (*it)->update(delta);
    }
}
void ObjectiveManager::addObjective(Objective * objective){
    objList.push_back(objective);
    objective->setID(objList.size());
}
Objective * ObjectiveManager::getObjective(std::string name){
    std::vector<Objective *>::iterator it;
    for(it = objList.begin(); it != objList.end(); it++){
        if((*it)->getName() == name){
            return (*it);
        }
    }
    return NULL;
}
Objective * ObjectiveManager::getObjective(unsigned int ID){
    std::vector<Objective *>::iterator it;
    for(it = objList.begin(); it != objList.end(); it++){
        if((*it)->getID() == ID){
            return (*it);
        }
    }
    return NULL;
}
//void ObjectiveManager::setPlayer(Player *playerOne){
//    player = playerOne;
//}
//void ObjectiveManager::setScene(Scene *scene){
//    currentScene = scene;
//}
//Player * ObjectiveManager::getPlayer(){
//    return player;
//}
//Scene * ObjectiveManager::getScene(){
//    return currentScene;
//}
bool ObjectiveManager::allComplete(){
    std::vector<Objective *>::iterator it;
    for(it = objList.begin(); it != objList.end(); it++){
        if(!(*it)->isComplete()){
            return false;
        }
    }
    return true;
}
void ObjectiveManager::clear(){
    std::vector<Objective *>::iterator it;
    for(it = objList.begin(); it != objList.end(); it++){
        delete (*it);
    }
}