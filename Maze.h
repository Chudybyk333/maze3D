#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shaderClass.h"

class Maze {
public:
    void LoadFromFile(const std::string& filename);
    void SetupRender();
    void Render(Shader& shader);

private:
    std::vector<std::string> map;
    int width = 0, height = 0;
    unsigned int cubeVAO = 0, cubeVBO = 0;
    int vertexCount = 0;
};
