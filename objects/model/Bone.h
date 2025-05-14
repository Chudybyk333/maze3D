#pragma once

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "./AssimpGLMHelpers.h"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    int numPositions;
    int numRotations;
    int numScalings;

    glm::mat4 localTransform;
    std::string name;
    int ID;

    void initPositions(const aiNodeAnim *channel);
    void initRotatings(const aiNodeAnim *channel);
    void initScalings(const aiNodeAnim *channel);

    float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    glm::mat4 interpolatePosition(float animationTime);
    glm::mat4 interpolateRotation(float animationTime);
    glm::mat4 interpolateScaling(float animationTime);

public:
    Bone(const std::string &name, int ID, const aiNodeAnim *channel);

    /*interpolates b/w positions, rotations & scaling keys based on the current time of
    the animation and prepares the local transformation matrix by combining all keys
    tranformations*/
    void update(float animationTime);

    glm::mat4 getLocalTransform();
    std::string getBoneName() const;
    int getBoneID();

    int getPositionIndex(float animationTime);

    int getRotationIndex(float animationTime);

    int getScaleIndex(float animationTime);
};