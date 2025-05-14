#include "Model.h"

Model::Model(
    std::string path,
    glm::vec3 position,
    glm::vec3 size,
    float rotationAngle,
    bool flipUvs,
    Shader shader)
    : shader{shader},
      pickerShader{ResourceManager::getShader("picker")},
      position{position},
      size{size},
      rotationAngle{rotationAngle}
{
    loadModel(path, flipUvs);
}

Model::Model(
    std::string path,
    std::vector<glm::mat4> instanceMatrices,
    bool flipUvs,
    Shader shader)
    : shader{shader},
      instanceMatrices{instanceMatrices},
      pickerShader{ResourceManager::getShader("picker")}
{
    loadModel(path, flipUvs);
}

Model::Model(
    std::string path,
    std::vector<glm::vec3> instancePositions,
    glm::vec3 instanceSize,
    bool flipUvs,
    Shader shader)
    : shader{shader},
      pickerShader{ResourceManager::getShader("picker")}
{
    for (auto pos : instancePositions)
    {
        glm::mat4 model{1.0f};
        model = glm::translate(model, pos);
        model = glm::scale(model, instanceSize);
        instanceMatrices.push_back(model);

        collisioners.push_back(Collisioner(glm::vec3{1.0f}, pos, Collisioner::BoundingBoxType::CUBE));
    }
    loadModel(path, flipUvs);
}

void Model::draw()
{
    shader.use();
    if (instanceMatrices.empty())
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->position);
        model = glm::rotate(model, glm::radians(this->rotationAngle), glm::vec3{0.0f, 1.0f, 0.0f});
        model = glm::scale(model, this->size);
        shader.setMatrixFloat4("model", model);
    }
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(std::string path, bool flipUvs)
{
    Assimp::Importer importer;
    // postprocessing options:
    // * aiProcess_GenNormals: generates normals for all vectors
    // * aiProcess_OptimizeMeshes: joins several meshes to reduce draw calls
    auto postprocessing = flipUvs ? (aiProcess_Triangulate | aiProcess_FlipUVs) : (aiProcess_Triangulate);
    const aiScene *scene = importer.ReadFile(path, postprocessing);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

glm::vec3 Model::getPosition() const
{
    return this->position;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // process children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ModelMesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        setVertexBoneDataToDefault(vertex);

        // process vertex positions, normals and texture coordinates
        vertex.position = AssimpGLMHelpers::getGLMVec(mesh->mVertices[i]);
        vertex.normal = AssimpGLMHelpers::getGLMVec(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    extractBoneWeightForVertices(vertices, mesh, scene);
    if (instanceMatrices.empty())
    {
        return ModelMesh(vertices, indices, textures);
    }
    else
    {
        return ModelMesh(instanceMatrices, vertices, indices, textures); // instancing
    }
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        MeshTexture texture;

        std::string filename = std::string(str.C_Str());
        std::string path = directory + '/' + filename;
        texture.id = ResourceManager::setTexture(path, path.c_str()).getId();
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

void Model::drawPicker(glm::vec3 id)
{
    this->pickerShader.use();
    this->pickerShader.setVec3Float("idCol", id);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::rotate(model, glm::radians(this->rotationAngle), glm::vec3{0.0f, 1.0f, 0.0f});
    model = glm::scale(model, this->size);
    pickerShader.setMatrixFloat4("model", model);
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawPicker(pickerShader);
    }
}

void Model::move(glm::vec3 direction)
{
    this->position += direction;
}

void Model::setRotation(float angle)
{
    this->rotationAngle = angle;
}

std::vector<Collisioner> Model::getCollisioners()
{
    return collisioners;
}

void Model::setVertexBoneDataToDefault(Vertex &vertex)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
    {
        vertex.boneIDs[i] = -1;
        vertex.weights[i] = 0.0f;
    }
}

void Model::setVertexBoneData(Vertex &vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
    {
        if (vertex.boneIDs[i] < 0)
        {
            vertex.weights[i] = weight;
            vertex.boneIDs[i] = boneID;
            break;
        }
    }
}

void Model::extractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCounter;
            newBoneInfo.offset = AssimpGLMHelpers::convertMatrixToGLMFormat(
                mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCounter;
            boneCounter++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            setVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}
