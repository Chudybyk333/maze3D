#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>

class Key {
public:
    Key();
    void LoadModel();
    void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    void SetPosition(const glm::vec3& position);
    glm::vec3 GetPosition() const;
    bool IsCollected() const;
    void Collect();

private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    glm::vec3 position;
    bool collected;
    std::vector<float> vertices;
};