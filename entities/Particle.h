#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../resources/ResourceManager.h"

class Particle
{
private:
    glm::vec3 position = glm::vec3{0.0f},
              velocity = glm::vec3{0.0f, 0.0f, 0.0f},
              size = glm::vec3{0.10f};
    glm::vec4 color = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
    float life = 0.0f;

    float initialLife = 1.0f;

public:
    Particle();
    bool isAlive();
    void update(float dt);
    void respawn(glm::vec3 position);
    glm::vec3 getPosition();
    glm::vec3 getSize();
};