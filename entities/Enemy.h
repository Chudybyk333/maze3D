#pragma once

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Entity.h"
#include "../Globals.h"
#include "./ParticleGenerator.h"

#include "../objects/model/Model.h"

#include "../objects/model/Animation.h"
#include "../objects/model/Animator.h"

class Enemy : public Entity
{
public:
    Enemy(
        float health,
        glm::ivec2 initialPos,
        std::vector<std::vector<MazeItem::Object>> grid,
        glm::vec3 gridBlockSize);
    ~Enemy();
    void update(float dt) override;
    void draw() override;
    void drawPicker(glm::vec3 id) override;

    void attack(float damage);
    bool isDead();

private:
    glm::vec3 calculateNewTargetPos();
    void updateNewPosition(float dt);
    bool inBounds(const glm::ivec2 &pos);
    void updateRotation();

    float health;
    float speed = 2.0f;

    glm::vec3 movingDir;
    glm::vec3 targetPos;
    float distanceToTravel = 0.0f;

    std::vector<std::vector<MazeItem::Object>> grid;
    glm::vec3 gridBlockSize;
    glm::ivec2 gridPosition;

    Model *model;
    Animation *animation;
    Animator *animator;
    ParticleGenerator particleGenerator;
};