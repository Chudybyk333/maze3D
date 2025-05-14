#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <exception>
#include <functional> // for std::bind

#include "stb_image.h"
#include "resources/shaders/Shader.h"
#include "resources/ResourceManager.h"
#include "objects/based/Plane.h"
#include "core/Player.h"
#include "core/Maze.h"
#include "utils/MazeLoader.h"
#include "utils/MazeGenerator.h"
#include "utils/ColorPicker.h"
#include "core/Skybox.h"
#include "objects/Crosshair.h"

class Game
{
private:
    GLFWwindow *window;

    int windowHeight = 0;
    int windowWidth = 0;

    unsigned int uboMatrices;

    float dt = 0.0f;

    Crosshair *crosshair;
    Player *player;
    Maze *maze;
    std::vector<Model *> lights{};
    Plane *ground;

    Skybox *skybox;

    void initGlfw();

    unsigned int pickerBuffer;
    void initPickerBuffer();

    void fillMaze();

    void render();
    void renderPickerBuffer();

    void handleMouseClick();
    void processInput();
    void processEvents();

    void handleMouse();

public:
    Game(int width, int height);
    ~Game();

    void mainloop();
};

struct FailedGLADInit : public std::exception
{
    const char *what() const throw()
    {
        return "Failed to initialize GLAD";
    }
};

struct FailedGLFWInit : public std::exception
{
    const char *what() const throw()
    {
        return "Failed to initialize GLFW";
    }
};