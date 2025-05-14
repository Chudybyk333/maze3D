#pragma once

#include "../objects/based/Cube.h"
#include "../objects/model/Model.h"
#include "../utils/ColorPicker.h"
#include "../entities/Enemy.h"

#include <vector>
#include <random>
#include <numeric>

class Maze
{
public:
    ~Maze();

    void draw();
    void update(const float dt);

    void drawPickerBuffer();
    std::vector<Collisioner> getCollisioners();
    std::vector<std::vector<MazeItem::Object>> getGrid();

    void addPickableModels(char *modelPath, const int amount, const bool flipUvs = true);
    void removePickableModel(int id, const glm::vec3 &playerPos, const float minDistance);
    void addEnemies(const int amount);
    void damageEnemy(int id, const glm::vec3 &playerPos, const float minDistance);

    // builder class
    class MazeBuilder
    {
    private:
        int width = 0;
        int height = 0;
        std::vector<std::vector<MazeItem::Object>> objects{};

    public:
        MazeBuilder(int width, int height);
        void addWall(int x, int y);
        Maze *build();
    };

private:
    std::vector<std::vector<MazeItem::Object>> objects;
    Model *cubes;
    std::vector<Model *> models{};
    std::vector<Enemy *> enemies{};
    ColorPicker *picker;
    bool backgroundMusicPlaying = false;

    Maze(std::vector<std::vector<MazeItem::Object>> objects);
    glm::mat4 getRandomizedModelMatrix(glm::vec3 position, glm::vec3 size = glm::vec3{1.0f}, bool flip = true);
    void addSpawnSurroundingCubes(std::vector<glm::vec3> &cubePositions);
    glm::ivec2 getRandomEmptyPos() const;
};