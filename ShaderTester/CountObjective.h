#ifndef COUNT_OBJECTIVE_H
#define COUNT_OBJECTIVE_H

#include "ObjectiveManager.h"

class CountObjective: public Objective
{
public:
    CountObjective(std::string name, float countLimit);
    ~CountObjective();
    void update(float delta);
    void addProgress(float progress = 1.0f);
    void setProgress(float progress);
    void setLimit(float limit);
    
    float getProgress();
    float getLimit();

private:
    float goal, current;
};

#endif