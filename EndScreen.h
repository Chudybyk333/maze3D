#pragma once
#include <glad/glad.h>
#include "Shader.h"

class EndScreen {
public:
    EndScreen();
    ~EndScreen();

    void Init(unsigned int textureID, Shader* shader);
    void Show();

private:
    unsigned int quadVAO, quadVBO;
    unsigned int texture;
    Shader* shader;

    void SetupQuad();
};
