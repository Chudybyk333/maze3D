#pragma once

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
};
