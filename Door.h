#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

class Door {
public:
    Door();
    void LoadModel();
    void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    void SetPosition(const glm::vec3& pos);
    glm::vec3 GetPosition() const;
    void Open();
    bool IsOpened() const;

private:
    unsigned int VAO, VBO, textureID;
    std::vector<float> vertices;
    glm::vec3 position;
    bool opened;
};
