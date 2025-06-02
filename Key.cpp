#include "key.h"
#include "texture.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Key::Key() : VAO(0), VBO(0), textureID(0), position(0.0f), collected(false) {}

void Key::LoadModel() {
    // Prosta kostka jako model klucza
    float size = 0.3f;
    vertices = {
        // przód
        -size, -size,  size,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         size, -size,  size,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         size,  size,  size,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
         size,  size,  size,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -size,  size,  size,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -size, -size,  size,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

        // ty³
        -size, -size, -size,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
         size, -size, -size,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
         size,  size, -size,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
         size,  size, -size,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        -size,  size, -size,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
        -size, -size, -size,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

        // llewo
        -size,  size,  size, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -size,  size, -size, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -size, -size, -size, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -size, -size, -size, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -size, -size,  size, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -size,  size,  size, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

        // prawo
         size,  size,  size,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         size,  size, -size,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         size, -size, -size,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         size, -size, -size,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         size, -size,  size,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         size,  size,  size,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

         // góra
         -size,  size, -size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
          size,  size, -size,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
          size,  size,  size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
          size,  size,  size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         -size,  size,  size,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         -size,  size, -size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

         // dó³
         -size, -size, -size,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
          size, -size, -size,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
          size, -size,  size,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
          size, -size,  size,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         -size, -size,  size,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         -size, -size, -size,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f
    };

    textureID = LoadTexture("key_texture.png");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Key::Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (collected) return;

    // Parametry œwiat³a klucza do shadera
    shader.use();
    shader.setVec3("keyLightPos", position + glm::vec3(0.0f, 0.2f, 0.0f)); // Lekko nad kluczem
    shader.setVec3("keyLightColor", lightColor);
    shader.setFloat("keyLightIntensity", lightIntensity);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f)); // Delikatne uniesienie
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f)); // Zmniejszenie rozmiaru
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
    glBindVertexArray(0);
}

void Key::SetPosition(const glm::vec3& position) {
    this->position = position;
}

glm::vec3 Key::GetPosition() const {
    return position;
}

bool Key::IsCollected() const {
    return collected;
}

void Key::Collect() {
    collected = true;
}