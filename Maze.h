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
    std::vector<AABB> doorColliders;
    const std::vector<AABB>& GetDoorColliders() const;
    void RemoveDoorColliderAt(const glm::vec3& position); // do późniejszego usuwania kolizji po otwarciu
    const std::vector<glm::vec3>& GetKeyPositions() const;
    const std::vector<glm::vec3>& GetLeftDoorPositions() const;
    const std::vector<glm::vec3>& GetRightDoorPositions() const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    unsigned int GetTextureID() const { return textureID; }
    unsigned int GenerateWallTexture();
    glm::vec3 GetBoundsMin() const { return glm::vec3(0, 0, 0); }
    glm::vec3 GetBoundsMax() const { return glm::vec3(width, 2.0f, height); }

private:
    std::vector<std::string> map;
    std::vector<glm::vec3> keyPositions;
    std::vector<glm::vec3> doorPositions;
    std::vector<glm::vec3> leftDoorPositions;
    std::vector<glm::vec3> rightDoorPositions;
    unsigned int textureID = 0;
    int width = 0, height = 0;
    unsigned int cubeVAO = 0, cubeVBO = 0;
    int vertexCount = 0;
    std::vector<AABB> colliders;
};