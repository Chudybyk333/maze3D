#include "ColorPicker.h"

ColorPicker::ColorPicker()
{
}

ColorPicker::~ColorPicker()
{
}

int ColorPicker::addModel(Model *m)
{
    this->modelMap[currModelId] = m;
    currModelId++;
    return currModelId - 1;
}

Model *ColorPicker::getModelByColor(int color)
{
    return this->modelMap[color];
}

void ColorPicker::removeModelByColor(int color)
{
    this->modelMap.erase(color);
}

void ColorPicker::draw()
{
    for (const auto &modelPair : this->modelMap)
    {
        modelPair.second->drawPicker(glm::vec3(modelPair.first, 0, 0));
    }
    for (const auto &enemyPair : this->enemyMap)
    {
        enemyPair.second->drawPicker(glm::vec3(0, enemyPair.first, 0));
    }
}

int ColorPicker::addEnemy(Enemy *e)
{
    this->enemyMap[currEnemyId] = e;
    currEnemyId++;
    return currEnemyId - 1;
}

Enemy *ColorPicker::getEnemyByColor(int color)
{
    return this->enemyMap[color];
}

void ColorPicker::removeEnemyByColor(int color)
{
    this->enemyMap.erase(color);
}
