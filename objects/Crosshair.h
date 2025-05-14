#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "../resources/shaders/Shader.h"
#include "../resources/ResourceManager.h"

class Crosshair
{
public:
    Crosshair(float width, float height);
    ~Crosshair();

    void draw();

private:
    Shader shader;
    std::vector<float> vertices;

    unsigned int VAO, VBO;
};