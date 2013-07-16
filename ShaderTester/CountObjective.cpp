#include "CountObjective.h"


CountObjective::CountObjective(std::string name, float countLimit): Objective(name)
{
    goal = countLimit;
    current = 0.0f;
    type = ObjectiveType::Count;
}


CountObjective::~CountObjective(void)
{
}

void CountObjective::update(float delta){
    if(current == goal){
        complete = true;
    }
}
float CountObjective::getProgress(){
    return ((float)current/(float)goal);
}
void CountObjective::setProgress(float progress){
    current = progress;
}
void CountObjective::addProgress(float progress){
    current += progress;
}
void CountObjective::setLimit(float limit){
    goal = limit;
}
float CountObjective::getLimit(){
    return goal;
}