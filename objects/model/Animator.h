#pragma once
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "./Bone.h"
#include "./Animation.h"

class Animator
{
public:
    Animator(Animation *animation);

    void updateAnimation(float dt);

    void playAnimation(Animation *pAnimation);
    void calculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform);
    std::vector<glm::mat4> getFinalBoneMatrices();

private:
    std::vector<glm::mat4> finalBoneMatrices;
    Animation *currentAnimation;
    float currentTime;
    float deltaTime;
};