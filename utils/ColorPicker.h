#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../objects/model/Model.h"
#include "../entities/Enemy.h"

#include <iostream>
#include <unordered_map>

/***
 * This class is not responsible for managing the lifetime of its objects
 */
class ColorPicker
{
public:
    ColorPicker();
    ~ColorPicker();

    int addModel(Model *m);
    int addEnemy(Enemy *e);

    Model *getModelByColor(int color);
    Enemy *getEnemyByColor(int color);

    void removeModelByColor(int color);
    void removeEnemyByColor(int color);
    void draw();

private:
    int currModelId = 1;
    int currEnemyId = 1;
    std::unordered_map<int, Model *> modelMap;
    std::unordered_map<int, Enemy *> enemyMap;
};