#pragma once
#include <vector>

#include "../Mesh.h"
#include "../../resources/ResourceManager.h"

class Cube : public Mesh
{
private:
    unsigned int VAO;
    std::vector<float> vertices;

    Shader shader;
    Texture diffuse;
    Texture specular;

    void initVertices();
    void initDefaultVaoVbo();

public:
    Cube(glm::vec3 position = glm::vec3{0.0f});
    Cube(std::vector<glm::vec3> instancePositions, glm::vec3 size = glm::vec3(1.0f));

    void draw() override;
};