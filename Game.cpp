#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Game.h"
#include "texture.h"
#include "maze.h"
#include "camera.h"
#include "Shader.h"
#include "ui.h"
#include <stb/stb_image.h>
#include <iostream>
#include <fstream>
#include "Skybox.h"
#include "Ground.h"
#include <vector>

GLFWwindow* window;
Camera camera;
Maze maze;
Shader* shader;
UI ui;
Shader* uiShader;

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*);

void Game::Init() {
    // Inicjalizacja GLFW
    if (!glfwInit()) {
        std::cerr << "Nie udało się zainicjować GLFW!" << std::endl;
        return;
    }

    // Ustawienia kontekstu OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pobierz monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Ustaw rozdzielczość okna na rozdzielczość monitora
    window = glfwCreateWindow(mode->width, mode->height, "Labirynt", monitor, NULL);
    if (window == NULL) {
        std::cerr << "Nie udało się stworzyć okna!" << std::endl;
        glfwTerminate();
        return;
    }

    // Włącz kontekst okna i ustaw callbacki
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Ukrycie kursora i wyłączenie jego interakcji
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Ładowanie funkcji OpenGL za pomocą GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Inicjalizacja shaderów i obiektów
    shader = new Shader("shader.vert", "shader.frag");
    maze.LoadFromFile("maze.txt");
    maze.SetupRender();
    camera.SetMaze(&maze);
    ground.SetupRender(maze);

    wallTextureID = maze.GenerateWallTexture();

    // Inicjalizacja kluczy
    for (const auto& pos : maze.GetKeyPositions()) {
        Key key;
        key.LoadModel();
        key.SetPosition(pos);
        keys.push_back(key);
    }
    // Inicjalizacja drzwi
    const auto& leftPositions = maze.GetLeftDoorPositions();
    const auto& rightPositions = maze.GetRightDoorPositions();
    // Upewnij się, że rozmiary się zgadzają
    size_t pairCount = std::min(leftPositions.size(), rightPositions.size());

    for (size_t i = 0; i < pairCount; ++i) {
        DoorPair pair;
        pair.Init(leftPositions[i], rightPositions[i]);
        pair.LoadModel();
        doorPairs.push_back(pair);
    }

    // Inicjalizacja shaderów UI
    uiShader = new Shader("ui.vert", "ui.frag");
    ui.Init(uiShader);

    // Wczytanie tekstur
    unsigned int handTex = LoadTexture("hand.png");
    ui.SetHandTexture(handTex);

    stbi_set_flip_vertically_on_load(true);

    // Initialize the skybox shader
    skyboxShader = new Shader("skybox.vert", "skybox.frag");

    // Load the skybox
    skybox = new Skybox();
    std::vector<std::string> faces = {
        "prawo.png", "lewo.png", "dol.png",
        "gora.png", "przod.png", "tyl.png"
    };

    if (!skybox->Load(faces)) {
        std::cerr << "Failed to load skybox!" << std::endl;
    }

    // Ustawienia oświetlenia
    lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientStrength = 0.5f;
}

bool Game::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void Game::ProcessInput() {
    processInput(window);
}

void Game::Update() {
    camera.Update();
    camera.UpdatePhysics(deltaTime);

    // Sprawdź kolizję z kluczami
    bool anyKeyCollected = false;

    for (auto& key : keys) {
        if (!key.IsCollected() &&
            glm::distance(camera.GetPosition(), key.GetPosition()) < 1.2f) {
            key.Collect();
            anyKeyCollected = true;
            // Tutaj możesz dodać efekt graficzny / dźwięk
        }
    }

    for (auto& pair : doorPairs) {
        pair.Update(deltaTime, camera.GetPosition(), keyCollected);
    }

    // Jednorazowo usuń kolizje po zebraniu klucza
    if (anyKeyCollected) {
        for (auto& pair : doorPairs)
            pair.RemoveCollidersFrom(maze);
        keyCollected = true; // dodaj pole w klasie Game, np. bool keyCollected = false;
    }
    camera.UpdatePhysics(deltaTime);
    ui.Update(deltaTime, camera.IsMoving(), !camera.IsJumping());
    camera.UpdatePrevPosition();
}

void Game::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sprawdź czy shadery są poprawne
    if (!shader->isCompiled() || !skyboxShader->isCompiled()) {
        std::cerr << "Błąd shadera!" << std::endl;
        return;
    }

    // Renderowanie sceny
    shader->use();



    // Przekazanie parametrów oświetlenia
    shader->setVec3("lightDir", lightDirection);
    shader->setVec3("lightColor", lightColor);
    shader->setVec3("viewPos", camera.GetPosition());
    shader->setFloat("ambientStrength", ambientStrength);

    // Przekaż rozmiar labiryntu i teksturę ścian
    shader->setVec3("mazeSize", glm::vec3(maze.GetWidth(), 2.0f, maze.GetHeight()));
    glActiveTexture(GL_TEXTURE1); // Użyj slotu tekstury 1 (0 jest zajęty przez texture_diffuse1)
    glBindTexture(GL_TEXTURE_2D, wallTextureID);
    shader->setInt("wallMap", 1);

    shader->setVec3("mazeBoundsMin", maze.GetBoundsMin());
    shader->setVec3("mazeBoundsMax", maze.GetBoundsMax());

    // Przekazanie macierzy widoku i projekcji
    shader->setMat4("view", camera.GetViewMatrix());
    shader->setMat4("projection", camera.GetProjectionMatrix());

    // Renderowanie światła klucza
    for (auto& key : keys) {
        key.Render(*shader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }

    // Renderowanie podłogi
    ground.Render(*shader);

    // Renderowanie labiryntu
    maze.Render(*shader);


    // Renderowanie kluczy
    for (auto& key : keys) {
        key.Render(*shader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }
    // Renderowanie drzwi
    for (auto& pair : doorPairs) {
        pair.Render(*shader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }


    // Renderowanie skyboxa
    if (skybox) {
        skybox->Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }

    // Renderowanie UI
    ui.Render();

    // Sprawdź błędy OpenGL
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::Cleanup() {
    delete skybox;
    skybox = nullptr;
    delete shader;
    delete uiShader;
    delete skyboxShader;
    glfwTerminate();
}

// --- Callbacki i input ---
void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    camera.OnMouseMove(window, xpos, ypos);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.ProcessKeyboard(window);
}