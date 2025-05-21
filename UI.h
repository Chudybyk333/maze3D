#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include <vector>
#include <string>

class Shader;

struct Item {
    unsigned int textureID;  // ID tekstury OpenGL
    std::string name;
};

class UI {
public:
    UI();
    ~UI();

    void Init(Shader* shader);
    void Update(float deltaTime, bool moving, bool jumping);
    void Render();
    void SetHandTexture(unsigned int texID);
    void SetInventory(const std::vector<Item>& items);
    void SetSelectedSlot(int slot);

private:
    Shader* shader;
    unsigned int quadVAO, quadVBO;

    unsigned int handTexture;
    std::vector<Item> inventory;
    int selectedSlot;
    float handAnimationTime = 0.0f;
    void SetupQuad();
    void RenderTexture(unsigned int texture, glm::vec2 position, glm::vec2 size, bool selected = false);
};
