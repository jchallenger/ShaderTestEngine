#ifndef UPGRADES_H
#define UPGRADES_H

#include <list>
#include <string>
#include "Singleton.h"


const static float ExchangeRate = 0.22f;
static int points;
static float currency;

class Skill
{
public:
    Skill(std::string name, int max, int cost);
    ~Skill();
    bool Upgrade();
    bool isMaxed();
    void setCost(int newCost);
    void reset();
    int getLevel();
    void setLevel(int lvl);
    std::string getId();
private:
    int level;
    int maxLevel;
    std::string ID;
    int cost, original_Cost;
};

class Upgrades : public Singleton<Upgrades>
{
private:
    std::list<Skill *> upgradeList;
public:
    Upgrades(void);
    ~Upgrades(void);

    Skill * getSkill(std::string);
    int getPoints();
    float getCurrency();

    void setCurrency(float _currency);
    void setPoints(int _points);
    void addPoints(int x = 1);
    void addSkill(std::string name, int maxLevel, int cost);

    void resetSkills();
};

#endif