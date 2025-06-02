#include "EndScreen.h"
#include <glm/vec2.hpp>

EndScreen::EndScreen() : quadVAO(0), quadVBO(0), texture(0), shader(nullptr) {}

EndScreen::~EndScreen() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void EndScreen::Init(unsigned int textureID, Shader* shader) {
    this->texture = textureID;
    this->shader = shader;
    SetupQuad();
}

void EndScreen::SetupQuad() {
    float vertices[] = {
        // pos    // tex
        0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
        0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

        0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        1.0f, 1.0f, 1.0f, 1.0f,  // top-right
        1.0f, 0.0f, 1.0f, 0.0f   // bottom-right
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void EndScreen::Show() {
    if (!shader) return;

    // Włącz blending alfa
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();

    glBindVertexArray(quadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("screenTexture", 0);

    // Ustaw uniform rect na fullscreen (0,0 do 1,1)
    shader->setVec4("rect", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    shader->setBool("selected", false); // Jeśli shader wymaga (z Twojego UI kodu)

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}
