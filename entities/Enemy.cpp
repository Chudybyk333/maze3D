#include "Enemy.h"

Enemy::Enemy(
    float health,
    glm::ivec2 initialPos,
    std::vector<std::vector<MazeItem::Object>> grid,
    glm::vec3 gridBlockSize)
    : Entity{glm::vec3(initialPos.x, 0.0f, initialPos.y) * gridBlockSize},
      health{health},
      grid{grid},
      gridBlockSize{gridBlockSize},
      gridPosition{initialPos},
      movingDir{0.0f, 0.0f, 0.0f},
      particleGenerator{100, ResourceManager::getTexture("smoke")},
      targetPos{this->position}
{
    model = new Model(
        "../assets/meshes/Amongus/scene.gltf",
        glm::vec3{this->position.x, -0.5f, this->position.z},
        glm::vec3{0.005f},
        0.0f,
        false,
        ResourceManager::getShader("meshAnimated"));
    animation = new Animation("../assets/meshes/Amongus/scene.gltf", model);
    animator = new Animator(animation);
}

Enemy::~Enemy()
{
    delete animator;
    delete animation;
    delete model;
}

void Enemy::update(float dt)
{
    // target reached
    if (this->distanceToTravel <= 0.0f)
    {
        // lock to target
        this->gridPosition = glm::ivec2(targetPos.x, targetPos.z);
        this->model->move(targetPos - position);
        this->position = targetPos;

        // find new target and set new moving direction
        this->targetPos = calculateNewTargetPos();
        this->distanceToTravel = glm::distance(this->targetPos, this->position);
        this->movingDir = this->targetPos - this->position;
        updateRotation();
    }
    updateNewPosition(dt);

    // update animation
    animator->updateAnimation(dt);
    // update particles
    glm::vec3 particlePosition = {this->position.x, this->position.y - 0.45f, this->position.z};
    particleGenerator.addParticles(dt, particlePosition);
    particleGenerator.update(dt);
}

void Enemy::draw()
{
    this->particleGenerator.draw();

    Shader shader = ResourceManager::getShader("meshAnimated");
    shader.use();
    auto transforms = animator->getFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i)
        shader.setMatrixFloat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
    this->model->draw();
}

void Enemy::drawPicker(glm::vec3 id)
{
    this->model->drawPicker(id);
}

glm::vec3 Enemy::calculateNewTargetPos()
{
    std::vector<glm::ivec2> possibleDirs = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::vector<glm::ivec2> possiblePos = {};
    for (auto dir : possibleDirs)
    {
        glm::ivec2 newPos = {this->gridPosition.y + dir.y, this->gridPosition.x + dir.x};
        if (inBounds(newPos) && grid[newPos.x][newPos.y] == MazeItem::Object::EMPTY)
        {
            possiblePos.push_back(newPos);
        }
    }
    glm::ivec2 newGridPos = possiblePos[rand() % possiblePos.size()];
    return glm::vec3(newGridPos.y, 0.0f, newGridPos.x);
}

void Enemy::updateNewPosition(float dt)
{
    glm::vec3 toIncrement = glm::vec3{movingDir.x, 0.0f, movingDir.z} * speed * dt;
    // make sure the max step size per move is one block length
    for (int i = 0; i < 3; i++)
    {
        if (toIncrement[i] > gridBlockSize[i])
            toIncrement[i] = gridBlockSize[i];
    }
    this->model->move(toIncrement);
    this->position += toIncrement;
    this->distanceToTravel -= glm::length(toIncrement);
}

bool Enemy::inBounds(const glm::ivec2 &pos)
{
    return pos.x >= 0 && pos.x < this->grid.size() && pos.y >= 0 && pos.y < this->grid[0].size();
}

void Enemy::updateRotation()
{
    float angle;
    float x = this->movingDir.x;
    float z = this->movingDir.z;
    if (x < 0.0f && z == 0.0f)
        angle = -90.0f;
    else if (x > 0.0f && z == 0.0f)
        angle = 90.0f;
    else if (x == 0.0f && z < 0.0f)
        angle = -180.0f;
    else
        angle = 0.0f;

    this->model->setRotation(angle);
}

void Enemy::attack(float damage)
{
    this->health -= damage;
}

bool Enemy::isDead()
{
    return this->health <= 0.0f;
}