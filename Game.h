#pragma once
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Key.h"
#include "Ground.h"
#include "Door.h"
#include "DoorWing.h"
#include "DoorPair.h"
#include "Portal.h"

// Forward declarations
class Shader;
class Skybox;
class Ground;

class Game {
public:
    void Init();
    void ProcessInput();
    void Update();
    void Render();
    void Cleanup();
    bool ShouldClose();
    bool keyCollected = false;
    bool opened = false;

private:
    float deltaTime = 0.016f;
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float ambientStrength;
    unsigned int wallTextureID;

    Skybox* skybox = nullptr;
    Shader* shader = nullptr;
    Shader* skyboxShader = nullptr;
    Shader* uiShader = nullptr;
    Shader* portalShader = nullptr;
    Ground ground;
    std::vector<Key> keys;
    std::vector<Door> doors;
};