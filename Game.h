#pragma once
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Forward declarations
class Shader;
class Skybox;
class Ground;

#include "ground.h"

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

    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float ambientStrength;

    // Obiekty gry
    Skybox* skybox = nullptr;
    Shader* shader = nullptr;
    Shader* skyboxShader = nullptr;
    Shader* uiShader = nullptr;
    Ground ground;
};