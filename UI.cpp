#include "ui.h"
#include "shaderClass.h"
#include <glad/glad.h>

UI::UI() : shader(nullptr), handTexture(0), selectedSlot(0) {}

UI::~UI() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void UI::Init(Shader* shader) {
    this->shader = shader;
    SetupQuad();
}

void UI::SetupQuad() {
    float vertices[] = {
        // pos      // tex
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
    // pozycja
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tekstura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void UI::SetHandTexture(unsigned int texID) {
    handTexture = texID;
}

void UI::SetInventory(const std::vector<Item>& items) {
    inventory = items;
}

void UI::SetSelectedSlot(int slot) {
    selectedSlot = slot;
}

void UI::RenderTexture(unsigned int texture, glm::vec2 position, glm::vec2 size, bool selected) {
    shader->use();
    glBindVertexArray(quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // ustaw uniformy pozycji i rozmiaru (np. jako vec4: x, y, width, height)
    shader->setVec4("rect", glm::vec4(position, size));

    // je?li slot jest zaznaczony, np. zmien kolor border
    shader->setBool("selected", selected);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void UI::Update(float deltaTime, bool moving, bool jumping) {
    if (moving || jumping) {
        handAnimationTime += deltaTime;
    }
    else {
        handAnimationTime = 0.0f;
    }
}

void UI::Render() {
    if (handTexture != 0) {
        float offsetY = -0.05f;
        if (handAnimationTime > 0.0f) {
            offsetY = -0.05f+0.015f * sin(handAnimationTime * 10.0f); // animacja ruchu r?ki
        }

        RenderTexture(handTexture, glm::vec2(0.7f, offsetY), glm::vec2(0.18f, 0.36f));
    }

    // render inventory etc.


    // Renderuj pasek ekwipunku normalnie
    if (inventory.size() > 0) {
        float slotWidth = 0.07f;
        float slotHeight = 0.07f;
        float startX = 0.5f - (inventory.size() * slotWidth) / 2.0f;
        float y = 0.05f;

        for (int i = 0; i < (int)inventory.size(); ++i) {
            bool selected = (i == selectedSlot);
            RenderTexture(inventory[i].textureID, glm::vec2(startX + i * slotWidth, y), glm::vec2(slotWidth, slotHeight), selected);
        }
    }
}

