#pragma once
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <iostream>
#include "./Model.h"
#include "./Bone.h"
#include "./BoneInfo.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
    float duration;
    int ticksPerSecond;
    std::vector<Bone> bones;
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;

    void readMissingBones(const aiAnimation *animation, Model &model);
    void readHierarchyData(AssimpNodeData &dest, const aiNode *src);

public:
    Animation(const std::string &animationPath, Model *model);

    ~Animation();

    Bone *findBone(const std::string &name);

    float getTicksPerSecond();
    float getDuration();
    const AssimpNodeData &getRootNode();

    const std::map<std::string, BoneInfo> &getBoneIDMap();
};