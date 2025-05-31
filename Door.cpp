#include "door.h"
#include "texture.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

Door::Door() : VAO(0), VBO(0), textureID(0), position(0.0f), opened(false) {}

void Door::LoadModel() {
    float size = 0.5f;
    vertices = {
        // Podobnie jak w Key.cpp — prosta kostka
        -size, -size,  size,  0, 0, 1,  0.0f, 0.0f,
         size, -size,  size,  0, 0, 1,  1.0f, 0.0f,
         size,  size,  size,  0, 0, 1,  1.0f, 1.0f,
         size,  size,  size,  0, 0, 1,  1.0f, 1.0f,
        -size,  size,  size,  0, 0, 1,  0.0f, 1.0f,
        -size, -size,  size,  0, 0, 1,  0.0f, 0.0f,

        -size, -size, -size,  0, 0, -1,  0.0f, 0.0f,
         size, -size, -size,  0, 0, -1,  1.0f, 0.0f,
         size,  size, -size,  0, 0, -1,  1.0f, 1.0f,
         size,  size, -size,  0, 0, -1,  1.0f, 1.0f,
        -size,  size, -size,  0, 0, -1,  0.0f, 1.0f,
        -size, -size, -size,  0, 0, -1,  0.0f, 0.0f,

        -size,  size,  size, -1, 0, 0,  1.0f, 0.0f,
        -size,  size, -size, -1, 0, 0,  1.0f, 1.0f,
        -size, -size, -size, -1, 0, 0,  0.0f, 1.0f,
        -size, -size, -size, -1, 0, 0,  0.0f, 1.0f,
        -size, -size,  size, -1, 0, 0,  0.0f, 0.0f,
        -size,  size,  size, -1, 0, 0,  1.0f, 0.0f,

         size,  size,  size, 1, 0, 0,  1.0f, 0.0f,
         size,  size, -size, 1, 0, 0,  1.0f, 1.0f,
         size, -size, -size, 1, 0, 0,  0.0f, 1.0f,
         size, -size, -size, 1, 0, 0,  0.0f, 1.0f,
         size, -size,  size, 1, 0, 0,  0.0f, 0.0f,
         size,  size,  size, 1, 0, 0,  1.0f, 0.0f,

        -size, -size, -size, 0, -1, 0,  0.0f, 1.0f,
         size, -size, -size, 0, -1, 0,  1.0f, 1.0f,
         size, -size,  size, 0, -1, 0,  1.0f, 0.0f,
         size, -size,  size, 0, -1, 0,  1.0f, 0.0f,
        -size, -size,  size, 0, -1, 0,  0.0f, 0.0f,
        -size, -size, -size, 0, -1, 0,  0.0f, 1.0f,

        -size,  size, -size, 0, 1, 0,  0.0f, 1.0f,
         size,  size, -size, 0, 1, 0,  1.0f, 1.0f,
         size,  size,  size, 0, 1, 0,  1.0f, 0.0f,
         size,  size,  size, 0, 1, 0,  1.0f, 0.0f,
        -size,  size,  size, 0, 1, 0,  0.0f, 0.0f,
        -size,  size, -size, 0, 1, 0,  0.0f, 1.0f,
    };

    textureID = LoadTexture("door.png");

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

void Door::Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (opened) return;

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
    glBindVertexArray(0);
}

void Door::SetPosition(const glm::vec3& pos) {
    position = pos;
}

glm::vec3 Door::GetPosition() const {
    return position;
}

void Door::Open() {
    opened = true;
}

bool Door::IsOpened() const {
    return opened;
}
