#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "game.h"
#include "texture.h"
#include "maze.h"
#include "camera.h"
#include "shaderClass.h"
#include "ui.h"
#include "stb_image.h"
#include <iostream>

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

    // Inicjalizacja shaderów i maze
    shader = new Shader("shader.vert", "shader.frag");
    maze.LoadFromFile("maze.txt");
    maze.SetupRender();
    camera.SetMaze(&maze);

    // Inicjalizacja shaderów UI
    uiShader = new Shader("ui.vert", "ui.frag");
    ui.Init(uiShader);

    // Wczytanie tekstur
    unsigned int handTex = LoadTexture("hand.png"); // zakładamy że masz tę funkcję

    ui.SetHandTexture(handTex);


    stbi_set_flip_vertically_on_load(true); // Tekstury są zwykle ładowane od góry do dołu w OpenGL
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
    ui.Update(deltaTime, camera.IsMoving(), !camera.IsJumping());
    ui.Render();
    camera.UpdatePrevPosition();
}

void Game::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setMat4("view", camera.GetViewMatrix());
    shader->setMat4("projection", camera.GetProjectionMatrix());

    maze.Render(*shader);
    ui.Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::Cleanup() {
    glfwTerminate();
    delete shader;
    delete uiShader;
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
