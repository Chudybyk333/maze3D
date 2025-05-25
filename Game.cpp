#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Game.h"
#include "texture.h"
#include "maze.h"
#include "camera.h"
#include "shaderClass.h"
#include "ui.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>

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

    // Skybox
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);  // WAŻNE: odłącz VAO

    // load textures
    std::vector<std::string> faces = {
        "prawo.png", 
        "lewo.png", 
        "gora.png",
        "dol.png", 
        "przod.png", 
        "tyl.png"
    };

    // Sprawdź czy pliki istnieją
    for (auto& path : faces) {
        std::ifstream file(path);
        if (!file.good()) {
            std::cerr << "Brak pliku: " << path << std::endl;
        }
    }

    cubemapTexture = LoadCubemap(faces);
    if (cubemapTexture == 0) {
        std::cerr << "Błąd ładowania skyboxa!" << std::endl;
    }
    std::cout << "Skybox zaladowany, ID: " << cubemapTexture << std::endl;

    // create shader
    skyboxShader = new Shader("skybox.vert", "skybox.frag");
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

    // 1. Najpierw sprawdź czy shadery są poprawne
    if (!shader->isCompiled() || !skyboxShader->isCompiled()) {
        std::cerr << "Błąd shadera!" << std::endl;
        return;
    }

    // 2. Renderowanie sceny
    shader->use();
    shader->setMat4("view", camera.GetViewMatrix());
    shader->setMat4("projection", camera.GetProjectionMatrix());
    maze.Render(*shader);

    // 3. Renderowanie skyboxa - ZMIENIONE
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();

    // Usuń translację z macierzy widoku
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", camera.GetProjectionMatrix());

    // Ustaw teksturę
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    skyboxShader->setInt("skybox", 0);  // WAŻNE: Ustawienie uniforma

    // Rysowanie
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);

    // 4. Renderowanie UI
    ui.Render();

    // 5. Sprawdź błędy OpenGL
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::Cleanup() {
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteTextures(1, &cubemapTexture);  // WAŻNE
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
