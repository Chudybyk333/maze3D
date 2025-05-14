#include "Animation.h"

Animation::Animation(const std::string &animationPath, Model *model)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    auto animation = scene->mAnimations[0];
    duration = animation->mDuration;
    ticksPerSecond = animation->mTicksPerSecond;
    readHierarchyData(rootNode, scene->mRootNode);
    readMissingBones(animation, *model);
}

Animation::~Animation()
{
}

void Animation::readMissingBones(const aiAnimation *animation, Model &model)
{
    int size = animation->mNumChannels;

    auto &boneInfoMap = model.getBoneInfoMap();
    int &boneCount = model.getBoneCount();

    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }

    this->boneInfoMap = boneInfoMap;
}

void Animation::readHierarchyData(AssimpNodeData &dest, const aiNode *src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::convertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        readHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

Bone *Animation::findBone(const std::string &name)
{
    auto iter = std::find_if(bones.begin(), bones.end(),
                             [&](const Bone &Bone)
                             {
                                 return Bone.getBoneName() == name;
                             });
    if (iter == bones.end())
        return nullptr;
    else
        return &(*iter);
}

float Animation::getTicksPerSecond()
{
    return ticksPerSecond;
}

float Animation::getDuration()
{
    return duration;
}

const AssimpNodeData &Animation::getRootNode()
{
    return rootNode;
}

const std::map<std::string, BoneInfo> &Animation::getBoneIDMap()
{
    return boneInfoMap;
}
