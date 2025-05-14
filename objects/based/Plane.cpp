#include "Plane.h"

Plane::Plane(glm::vec3 position, float sizeScalar, float textureSize)
    : Mesh{glm::vec3{position}, glm::vec3{sizeScalar}},
      shader{ResourceManager::getShader("default")},
      diffuse{ResourceManager::getTexture("groundDiffuse")},
      specular{ResourceManager::getTexture("groundSpecular")}
{
    vertices = {
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f * sizeScalar / textureSize, 1.0f * sizeScalar / textureSize, // 1
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f * sizeScalar / textureSize, 0.0f * sizeScalar / textureSize,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f * sizeScalar / textureSize, 0.0f * sizeScalar / textureSize, // 2
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f * sizeScalar / textureSize, 1.0f * sizeScalar / textureSize
    };

    indices = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Plane::draw()
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::scale(model, this->size);
    shader.setMatrixFloat4("model", model);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    shader.setInt("material.texture_diffuse1", 0);
    shader.setInt("material.texture_specular1", 1);
    diffuse.bindTexture(GL_TEXTURE0);
    specular.bindTexture(GL_TEXTURE1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
