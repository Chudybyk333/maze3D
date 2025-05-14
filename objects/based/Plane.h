#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../Mesh.h"
#include "../../resources/ResourceManager.h"

class Plane : public Mesh
{
private:
    unsigned int VAO;
    unsigned int EBO;
    std::vector<float> vertices;
    std::vector<int> indices;

    float textureRatio;

    Shader shader;
    Texture diffuse;
    Texture specular;

public:
    Plane(
        glm::vec3 position = glm::vec3{0.0f},
        float sizeScalar = 100.0f,
        float textureSize = 1.0f);
    void draw() override;
};