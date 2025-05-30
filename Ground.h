#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include "maze.h"

class Ground {
public:
    Ground();
    void Render(Shader& shader);
    void SetupRender(const Maze& maze);

private:
    unsigned int groundVAO, groundVBO;
    unsigned int textureID;
    int vertexCount;
};