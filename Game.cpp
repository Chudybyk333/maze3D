﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Game.h"
#include "texture.h"
#include "maze.h"
#include "camera.h"
#include "Shader.h"
#include "EndScreen.h"
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
Shader* endShader;
Shader* portalShader;
Portal portal;
Door door;
EndScreen endScreen;

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
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Pobranie monitora
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Ustawienie rozdzielczości okna na rozdzielczość monitora
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
    size_t pairCount = std::min(leftPositions.size(), rightPositions.size());

    for (size_t i = 0; i < pairCount; ++i) {
        door.Init(leftPositions[i], rightPositions[i]);
        door.LoadModel();
        doors.push_back(door);
    }
    portalShader = new Shader("portal.vert", "portal.frag");


    // Inicjalizacja shaderów UI
    uiShader = new Shader("ui.vert", "ui.frag");
    ui.Init(uiShader);

    // Inicjalizacja shaderów endscreen
    endShader = new Shader("ui.vert", "ui.frag");

    // Wczytanie tekstur
    unsigned int handTex = LoadTexture("hand.png");
    ui.SetHandTexture(handTex);

    stbi_set_flip_vertically_on_load(true);

    // Inicjalizacja skybox shader
    skyboxShader = new Shader("skybox.vert", "skybox.frag");

    // Załadowanie skyboxa
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

    //czas
    startTime = glfwGetTime();


    //End screen
    unsigned endTexture = LoadTexture("end_screen.png");
    endScreen.Init(endTexture,endShader);

}

bool Game::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void Game::ProcessInput() {
    processInput(window);
}

void Game::Update() {
    if (!gameFinished) {
        camera.Update();
        camera.UpdatePhysics(deltaTime);
    }
    else {
        camera.UpdatePrevPosition();
    }

    for (auto& key : keys) {
        if (!key.IsCollected() &&
            glm::distance(camera.GetPosition(), key.GetPosition()) < 1.2f) {
            key.Collect();
        }
    }

    // Sprawdzenie czy klucze zostały zebrane
    if (!allKeysCollected) {
        allKeysCollected = true;
        for (const auto& key : keys) {
            if (!key.IsCollected()) {
                allKeysCollected = false;
                break;
            }
        }
    }
    if (allKeysCollected) {
        for (auto& door : doors) {
            door.RemoveCollidersFrom(maze);
        }
    }
    for (auto& door : doors) {
        door.Update(deltaTime, camera.GetPosition(), allKeysCollected);
    }

    if (!gameFinished && allKeysCollected && glm::distance(camera.GetPosition(), door.GetPortal().GetPosition()) < 1.6f) {
        endTime = glfwGetTime();
        gameFinished = true;
        std::cout << "Game finished";

    }

    camera.UpdatePhysics(deltaTime);
    ui.Update(deltaTime, camera.IsMoving(), !camera.IsJumping());
    camera.UpdatePrevPosition();

}

void Game::Render() {
    if (gameFinished) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
        return;
    }


    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sprawdzenie czy shadery są poprawne
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

    //portal swiatlo parametry
    for (const auto& door : doors) {
        const Portal& portal = door.GetPortal();
        if (portal.IsVisible()) {
            shader->setVec3("portalPos", portal.GetPosition());
            shader->setFloat("portalIntensity", portal.GetIntensity());
            break;
        }
    }

    // Przekazanie tekstury labiryntu i ścian
    shader->setVec3("mazeSize", glm::vec3(maze.GetWidth(), 2.0f, maze.GetHeight()));
    glActiveTexture(GL_TEXTURE1);
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
    shader->setFloat("keyLightIntensity", 0.0f);

    for (auto& key : keys) {
        key.Render(*shader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }
    // Renderowanie drzwi
    for (auto& door : doors) {
        door.Render(*shader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }
    // Renderowanie portalu
    for (auto& door : doors) {
        door.RenderPortal(*portalShader, camera.GetViewMatrix(), camera.GetProjectionMatrix());
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

// Callbacki i input
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