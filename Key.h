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

    // Dodane w³aœciwoœci œwiat³a
    glm::vec3 GetLightColor() const { return lightColor; }
    float GetLightIntensity() const { return lightIntensity; }

private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    glm::vec3 position;
    bool collected;
    std::vector<float> vertices;

    // W³aœciwoœci œwiat³a klucza
    glm::vec3 lightColor = glm::vec3(1.0f, 0.8f, 0.2f); // Z³oty kolor
    float lightIntensity = 1.5f;
};