#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Maze {
public:
    void LoadFromFile(const std::string& filename);
    void SetupRender();
    void Render(Shader& shader);
    const std::vector<AABB>& GetColliders() const;
    const std::vector<glm::vec3>& GetKeyPositions() const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    unsigned int GetTextureID() const { return textureID; }

private:
    std::vector<std::string> map;
    std::vector<glm::vec3> keyPositions;
    unsigned int textureID = 0;
    int width = 0, height = 0;
    unsigned int cubeVAO = 0, cubeVBO = 0;
    int vertexCount = 0;
    std::vector<AABB> colliders;
};