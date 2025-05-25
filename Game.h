#pragma once

// Forward declaration of Shader class
class Shader;

class Game {
public:
    void Init();
    void ProcessInput();
    void Update();
    void Render();
    void Cleanup();
    bool ShouldClose();
private:
    float deltaTime = 0.016f;
    unsigned int skyboxVAO = 0; // Initialize to 0
    unsigned int skyboxVBO = 0; // Initialize to 0
    unsigned int cubemapTexture = 0; // Initialize to 0
    Shader* skyboxShader = nullptr; // Initialize to nullptr
};
