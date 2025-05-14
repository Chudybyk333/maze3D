#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/epsilon.hpp>
#include <iostream>
#include <cmath>

// Collisioner is always a box shape
class Collisioner
{
public:
    enum class BoundingBoxType
    {
        SPHERE,
        CUBE,
        NONE
    };

    Collisioner() = default;
    Collisioner(glm::vec3 size, glm::vec3 center, BoundingBoxType boxType);
    ~Collisioner();

    void setCenter(glm::vec3 center);
    glm::vec3 getCenter();
    glm::vec3 getMinVec();
    glm::vec3 getMaxVec();

    glm::vec3 minVec;
    glm::vec3 maxVec;

    bool isColliding(Collisioner col);
    glm::vec3 getVectorToTranslate(Collisioner col);

    BoundingBoxType getType();

    glm::vec3 size;

private:
    glm::vec3 center;
    BoundingBoxType boxType = BoundingBoxType::NONE;
};