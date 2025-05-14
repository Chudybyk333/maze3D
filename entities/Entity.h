#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Entity
{
public:
    Entity() = default;
    Entity(glm::vec3 position);
    ~Entity() = default;

    glm::vec3 getPosition() const { return this->position; };

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void drawPicker(glm::vec3 id){};

protected:
    glm::vec3 position;
};