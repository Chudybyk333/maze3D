#pragma once

#include <vector>
#include <ranges>
#include "../../resources/ResourceManager.h"
#include "../../stb_image.h"

// same as in shader (mesh.vert)
#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class ModelMesh
{
private:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;
    unsigned int VAO;
    unsigned int VBO, EBO;

    std::vector<glm::mat4> instanceModelMatrices = {};

    void setupMesh();

public:
    ModelMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    ModelMesh(std::vector<glm::mat4> instanceModelMatrices, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    void draw(Shader &shader);
    void drawPicker(Shader &shader);
};