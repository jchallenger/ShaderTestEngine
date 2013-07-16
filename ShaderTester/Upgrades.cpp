#include "Upgrades.h"



Skill::Skill(std::string name, int max, int _cost){
    ID = name;
    maxLevel = max;
    cost = _cost;
    original_Cost = cost;
    level = 0;
}
Skill::~Skill(){ 

}
void Skill::reset(){
    cost = original_Cost;
    setLevel(0);
}
bool Skill::Upgrade(){
    if(level <= maxLevel && Upgrades::getInstance()->getCurrency() >= cost){
        Upgrades::getInstance()->setCurrency(Upgrades::getInstance()->getCurrency() - cost);
        level++;
        return true;
    }
    return false;
}
void Skill::setCost(int newCost){
    if(newCost >= 0)
        cost = newCost;
    else
        cost = 0;
}
bool Skill::isMaxed(){
    return (level == maxLevel);
}
int Skill::getLevel(){
    return level;
}
void Skill::setLevel(int lvl){
    if(lvl > maxLevel)
        level = maxLevel;
    else if(lvl < 0)
        level = 0;
    else
        level = lvl;
}
std::string Skill::getId(){
    return ID;
}

Upgrades::Upgrades(void)
{
    points = 0;
    currency = 0;
}

Upgrades::~Upgrades(void)
{
}
void Upgrades::addSkill(std::string name, int maxLevel, int cost){
    upgradeList.push_back(new Skill(name,maxLevel,cost));
}
Skill * Upgrades::getSkill(std::string name){
    std::list<Skill *>::iterator it;
    for(it = upgradeList.begin(); it != upgradeList.end(); it++){
        if(name == (*it)->getId() ){
            return (*it);
        }
    }
    return NULL;
}
void Upgrades::resetSkills(){
    points = 0;
    currency = 0.0f;
    std::list<Skill *>::iterator it;
    for(it = upgradeList.begin(); it != upgradeList.end(); it++){
        (*it)->reset();
    }
}
void Upgrades::setCurrency(float _currency){
    currency = _currency;
}
void Upgrades::setPoints(int _points){
    points = _points;
}
void Upgrades::addPoints(int x){
    points += x;
    currency += x * ExchangeRate;
}
int Upgrades::getPoints(){
    return points;
}
float Upgrades::getCurrency(){
    return currency;
}
