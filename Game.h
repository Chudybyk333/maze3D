#pragma once

// Forward declarations
class Shader;
class Skybox;  // Dodaj forward declaration dla Skybox

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


    Skybox* skybox = nullptr;  
    Shader* shader = nullptr;  
    Shader* skyboxShader = nullptr;
};