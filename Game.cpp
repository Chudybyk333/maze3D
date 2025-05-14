#include "Game.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

Game::Game(int width, int height)
    : windowWidth{width},
      windowHeight{height}
{
    initGlfw();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw FailedGLADInit();
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    stbi_set_flip_vertically_on_load(true); // before loading any image

    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_CULL_FACE);  // can't see inside of faces

    glEnable(GL_BLEND); // enable so we can use alpha values in fragment shaders
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(0); // disable vsync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(10.0f, 3.0f, 5.0f),
        glm::vec3(10.0f, 3.0f, 15.0f)
    };

    ResourceManager::initShaders(pointLightPositions);
    ResourceManager::initTextures();
    ResourceManager::addSound("eating", "../assets/audio/eatingSound.wav");
    ResourceManager::addSound("damage", "../assets/audio/damageSound.wav");
    ResourceManager::addSound("space-jazz", "../assets/audio/space-jazz.mp3");

    // init shader matrices buffer
    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->player = new Player();

    this->crosshair = new Crosshair(1.0f / windowWidth * 20.0f, 1.0f / windowHeight * 20.0f);

    // this->maze = MazeLoader().loadMazeFromFile("../assets/maze.txt");
    this->maze = MazeGenerator().getMaze();
    fillMaze();

    this->ground = new Plane(glm::vec3{0.0f, -0.5f, 0.0f}, 100.0f, 1.0f);

    // this->model = new Model("../assets/meshes/Tree/Tree.obj", glm::vec3{0.0f, -0.5f, 0.0f});
    this->skybox = new Skybox();

    this->lights.push_back(new Model("../assets/meshes/Fantasy/Lantern/LanternLit.obj", pointLightPositions[0]));
    this->lights.push_back(new Model("../assets/meshes/Fantasy/Lantern/LanternLit.obj", pointLightPositions[1]));

    initPickerBuffer();
}

Game::~Game()
{
    delete crosshair;
    delete player;
    delete maze;
    for (auto light : lights)
    {
        delete light;
    }
    delete skybox;

    glfwTerminate();
}

void Game::initGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    this->window = glfwCreateWindow(windowWidth, windowHeight, "Gaze Maze", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw FailedGLFWInit();
    }
    glfwMakeContextCurrent(window);
}

void Game::mainloop()
{
    float oldTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float newTime = glfwGetTime();
        this->dt = newTime - oldTime;
        oldTime = newTime;
        float fps = 1 / dt;
        // std::cout<< "FPS: " << fps << std::endl;

        processInput();
        processEvents();

        glm::mat4 view = player->getView();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glm::vec3 viewPos = player->getPosition();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(viewPos));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        render();
        // check and call events and swap the buffers
        // everything after this swap that is rendered will not be seen
        glfwPollEvents();
        glfwSwapBuffers(window);

        renderPickerBuffer();
    }
}

void Game::render()
{
    glClearColor(37.0f / 255.0f, 124.0f / 255.0f, 190.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // fills the screen with the color configured by glClearColor, and clears the depth buffer bit

    // handle render calls here
    maze->draw();
    ground->draw();
    skybox->draw();
    for (auto light : lights)
    {
        light->draw();
    }
    player->draw(); // for particles
    crosshair->draw();
}

void Game::renderPickerBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->pickerBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->maze->drawPickerBuffer();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::processInput()
{
    handleMouse();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player->handleKeyInput(Player::InputEvent::FORWARDS);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player->handleKeyInput(Player::InputEvent::BACKWARDS);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        player->handleKeyInput(Player::InputEvent::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player->handleKeyInput(Player::InputEvent::RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        player->handleKeyInput(Player::InputEvent::JUMP);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        delete this->maze;
        this->maze = MazeLoader().loadMazeFromFile("../assets/maze.txt");
        fillMaze();
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        delete this->maze;
        this->maze = MazeGenerator().getMaze();
        fillMaze();
    }
}

void Game::processEvents()
{
    player->update(this->dt);
    player->doCollisions(this->maze->getCollisioners());
    maze->update(this->dt);
}

void Game::handleMouse()
{
    GLdouble xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    player->setDirectionByMouse((float)xPos, (float)yPos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        handleMouseClick();
    }
}

void Game::handleMouseClick()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    double midx = width / 2.0f;
    double midy = height / 2.0f;

    unsigned char pixel[4];
    glReadPixels(midx, midy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    // std::cout << "Clicked color: (" << static_cast<int>(pixel[0]) << ", " << static_cast<int>(pixel[1]) << ", " << static_cast<int>(pixel[2]) << ")" << std::endl;

    if (static_cast<int>(pixel[0]) != 0)
    {
        this->maze->removePickableModel(static_cast<int>(pixel[0]), this->player->getPosition(), 1.25f);
    }
    else if (static_cast<int>(pixel[1]) != 0)
    {
        this->maze->damageEnemy(static_cast<int>(pixel[1]), this->player->getPosition(), 5.0f);
    }
}

void Game::initPickerBuffer()
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, pickerBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::fillMaze()
{
    this->maze->addPickableModels("../assets/meshes/Fantasy/Shroom/Mushroom.obj", 20, false);
    this->maze->addEnemies(4);
}
