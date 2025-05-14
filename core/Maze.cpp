#include "Maze.h"

Maze::Maze(std::vector<std::vector<MazeItem::Object>> objects)
    : objects{objects},
      picker{new ColorPicker()}
{
    std::vector<glm::vec3> cubePositions = {};
    std::vector<glm::mat4> rockPositions = {};
    std::vector<glm::mat4> rockVarPositions = {};
    std::vector<glm::mat4> lowPolyPlantPositions = {};
    std::vector<glm::mat4> grassSpotPositions = {};
    // init models
    for (int row = 0; row < objects.size(); row++)
    {
        for (int col = 0; col < objects[row].size(); col++)
        {
            MazeItem::Object obj = objects[row][col];
            if (obj == MazeItem::Object::WALL)
            {
                auto cubePos = glm::vec3(col, 0, row);
                cubePositions.push_back(cubePos);
            }
            else if (obj == MazeItem::Object::EMPTY)
            {
                int randomNum = rand() % 4;
                if (randomNum == 0)
                {
                    rockPositions.push_back(getRandomizedModelMatrix(glm::vec3{col, -0.35f, row}, glm::vec3{0.3f}, false));
                }
                else if (randomNum == 1)
                {
                    rockVarPositions.push_back(getRandomizedModelMatrix(glm::vec3{col, -0.35f, row}, glm::vec3{0.3f}, false));
                }
                int placeFlower = rand() % 2;
                if (placeFlower)
                {
                    lowPolyPlantPositions.push_back(getRandomizedModelMatrix(glm::vec3{col, -0.55f, row}, glm::vec3{0.1f}, false));
                }
                grassSpotPositions.push_back(getRandomizedModelMatrix(glm::vec3{col, -0.5f, row}));
                grassSpotPositions.push_back(getRandomizedModelMatrix(glm::vec3{col, -0.5f, row}));
            }
        }
    }
    models.push_back(new Model("../assets/meshes/Rocks/Rocks.dae", rockPositions, false));
    models.push_back(new Model("../assets/meshes/RocksVar1/RocksVar1.dae", rockVarPositions, false));
    models.push_back(new Model("../assets/meshes/LowPolyPlant/LowPolyPlant.dae", lowPolyPlantPositions, false));
    models.push_back(new Model("../assets/meshes/grassSpot/grassSpot.obj", grassSpotPositions, false));
    addSpawnSurroundingCubes(cubePositions);
    cubes = new Model{"../assets/meshes/Wall/Wall.dae", cubePositions, glm::vec3{0.5f}};
}

glm::mat4 Maze::getRandomizedModelMatrix(glm::vec3 position, glm::vec3 size, bool flip)
{
    float x = (float)rand() / RAND_MAX - 0.5f;
    float z = (float)rand() / RAND_MAX - 0.5f;
    float sizeOffset = (float)rand() / RAND_MAX - 0.5f;
    float rotation = (float)rand() / RAND_MAX * 360.0f;
    glm::vec3 newPosition(position.x + x, position.y, position.z + z);
    glm::vec3 newSize = size + glm::vec3{(sizeOffset / 2.0f) / 10.0f};
    glm::mat4 model{1.0f};
    model = glm::translate(model, newPosition);
    model = glm::scale(model, newSize);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3{0.0f, 1.0f, 0.0f});
    return model;
}

void Maze::addSpawnSurroundingCubes(std::vector<glm::vec3> &cubePositions)
{
    std::vector<glm::vec3> positions = {
        {-1.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {1.0f, 0.0f, -1.0f},
        {2.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 1.0f},
        {-1.0f, 0.0f, 2.0f},
    };
    for (auto pos : positions)
    {
        cubePositions.push_back(pos);
    }
}

Maze::~Maze()
{
    delete cubes;
    for (auto model : models)
        delete model;
    delete picker;
    for (auto enemy : enemies)
        delete enemy;
}

void Maze::draw()
{
    cubes->draw();
    for (const auto &model : models)
    {
        model->draw();
    }
    for (const auto &enemy : enemies)
    {
        enemy->draw();
    }
}

void Maze::update(const float dt)
{
    for (const auto &enemy : enemies)
    {
        enemy->update(dt);
    }
}

Maze::MazeBuilder::MazeBuilder(int width, int height)
    : width{width},
      height{height},
      objects{
          height,
          std::vector{width, MazeItem::Object::EMPTY}}
{
}

void Maze::MazeBuilder::addWall(int x, int y)
{
    objects[y][x] = MazeItem::Object::WALL;
}

Maze *Maze::MazeBuilder::build()
{
    objects[0][0] = MazeItem::Object::EMPTY;
    objects[0][1] = MazeItem::Object::EMPTY;
    objects[1][0] = MazeItem::Object::EMPTY;
    objects[height - 1][width - 1] = MazeItem::Object::EMPTY;
    objects[height - 1][width - 2] = MazeItem::Object::EMPTY;
    objects[height - 2][width - 1] = MazeItem::Object::EMPTY;
    return new Maze(objects);
}

std::vector<Collisioner> Maze::getCollisioners()
{
    return cubes->getCollisioners();
}

void Maze::addPickableModels(char *modelPath, const int amount, const bool flipUvs)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distPos(-0.25, 0.25);
    std::uniform_int_distribution<> distRot(0, 359);

    // shuffle indices
    std::vector<glm::vec2> allCoords{};
    for (int i = 0; i < objects.size(); i++)
    {
        for (int j = 0; j < objects[0].size(); j++)
        {
            allCoords.push_back(glm::vec2{j, i});
        }
    }
    std::random_shuffle(allCoords.begin(), allCoords.end());

    int count = 0;
    for (auto coord : allCoords)
    {
        if (count == amount)
            return;

        MazeItem::Object obj = objects[coord.y][coord.x];
        if (obj == MazeItem::Object::EMPTY)
        {
            float x = distPos(gen);
            float z = distPos(gen);
            float r = distRot(gen);

            Model *m = new Model(modelPath, glm::vec3(coord.x + x, -0.43f, coord.y + z),
                                 glm::vec3(0.3f, 0.3f, 0.3f),
                                 r, flipUvs, ResourceManager::getShader("mesh"));
            models.push_back(m);
            picker->addModel(m);
            ++count;
        }
    }
}

void Maze::removePickableModel(int id, const glm::vec3 &playerPos, const float minDistance)
{
    Model *m = this->picker->getModelByColor(id);
    if (m == nullptr)
    {
        std::cout << "Could not find object with id " << id << std::endl;
        return;
    }

    if (glm::distance2(playerPos, m->getPosition()) > minDistance * minDistance)
        return;

    auto it{std::find_if(models.begin(), models.end(), [m](const Model *model)
                         { return m == model; })};
    if (it != models.end())
    {
        auto idx{it - models.begin()};
        this->models.erase(models.begin() + idx);
        this->picker->removeModelByColor(id);
        delete m;
    }
    ResourceManager::playSound("eating");
    if (!backgroundMusicPlaying)
    {
        ResourceManager::playSound("space-jazz", 0.2f, true);
        backgroundMusicPlaying = true;
    }
}

void Maze::drawPickerBuffer()
{
    this->picker->draw();
}

glm::ivec2 Maze::getRandomEmptyPos() const
{
    std::vector<glm::ivec2> empty = {};
    for (int row = 0; row < objects.size(); row++)
    {
        for (int col = 0; col < objects[row].size(); col++)
        {
            if (objects[row][col] == MazeItem::Object::EMPTY)
            {
                empty.push_back(glm::ivec2(col, row));
            }
        }
    }
    int randomPos = rand() % empty.size();
    return empty[randomPos];
}

void Maze::addEnemies(const int amount)
{
    for (int i = 0; i < amount; i++)
    {
        Enemy *e = new Enemy(100.0f, getRandomEmptyPos(), objects, glm::vec3(1.0f, 1.0f, 1.0f));
        this->enemies.push_back(e);
        this->picker->addEnemy(e);
    }
}

void Maze::damageEnemy(int id, const glm::vec3 &playerPos, const float minDistance)
{
    Enemy *e = this->picker->getEnemyByColor(id);
    if (e == nullptr)
    {
        std::cout << "Could not find enemy with id " << id << std::endl;
        return;
    }

    if (glm::distance2(playerPos, e->getPosition()) > minDistance * minDistance)
        return;

    auto it{std::find_if(enemies.begin(), enemies.end(), [e](const Enemy *enemy)
                         { return e == enemy; })};
    if (it != enemies.end())
    {
        auto idx{it - enemies.begin()};
        e->attack(100.0f);
        if (e->isDead())
        {
            this->enemies.erase(enemies.begin() + idx);
            this->picker->removeEnemyByColor(id);
            delete e;
        }
        ResourceManager::playSound("damage");
    }
}
