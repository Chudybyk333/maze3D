#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../resources/shaders/Shader.h"
#include "../resources/ResourceManager.h"
#include <string>
#include <vector>

class Skybox
{
public:
    Skybox();
    ~Skybox();

    void draw();

private:
    std::vector<float> skyboxVertices;
    std::vector<unsigned int> skyboxIndices;
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int cubemapTexture;

    Shader shader;
    std::string parentDir = "../assets/skybox/";
    std::string cubemap[6] = {
        parentDir + "right.png",
        parentDir + "left.png",
        parentDir + "top.png",
        parentDir + "bottom.png",
        parentDir + "front.png",
        parentDir + "back.png"};
};