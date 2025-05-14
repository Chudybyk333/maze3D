#pragma once
#include <string>
#include <vector>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "../Collisioner.h"
#include "./ModelMesh.h"
#include "./AssimpGLMHelpers.h"
#include "./BoneInfo.h"

#define MAX_BONE_WEIGHTS 4

class Model
{
public:
    Model(
        std::string path,
        glm::vec3 position = glm::vec3{0.0f},
        glm::vec3 size = glm::vec3{1.0f},
        float rotationAngle = 0.0f,
        bool flipUvs = true,
        Shader shader = {ResourceManager::getShader("mesh")});
    Model(
        std::string path,
        std::vector<glm::mat4> instanceMatrices,
        bool flipUvs = true,
        Shader shader = {ResourceManager::getShader("meshInstancing")});
    Model(
        std::string path,
        std::vector<glm::vec3> instancePositions,
        glm::vec3 instanceSize = glm::vec3{1.0f},
        bool flipUvs = true,
        Shader shader = {ResourceManager::getShader("meshInstancing")});
    void draw();
    void drawPicker(glm::vec3 id);
    void move(glm::vec3 direction);
    glm::vec3 getPosition() const;

    auto &getBoneInfoMap() { return boneInfoMap; }
    int &getBoneCount() { return boneCounter; }

    void setRotation(float angle);

    std::vector<Collisioner> getCollisioners();

private:
    Shader shader;
    Shader pickerShader;
    glm::vec3 position = glm::vec3{0.0f}, size = glm::vec3{1.0f};
    float rotationAngle;

    std::vector<ModelMesh> meshes;
    std::string directory;

    std::vector<glm::mat4> instanceMatrices;

    void loadModel(std::string path, bool flipUvs);
    void processNode(aiNode *node, const aiScene *scene);
    ModelMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    // collisioning
    std::vector<Collisioner> collisioners{};

    // bones
    std::map<std::string, BoneInfo> boneInfoMap;
    int boneCounter = 0;

    void setVertexBoneDataToDefault(Vertex &vertex);
    void setVertexBoneData(Vertex &vertex, int boneID, float weight);
    void extractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene);
};