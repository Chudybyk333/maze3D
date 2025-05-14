#pragma once
#include <map>
#include <string>
#include <irrklang/irrKlang.h>

#include "textures/Texture.h"
#include "shaders/Shader.h"

class ResourceManager
{
private:
    ResourceManager() = default;
    static irrklang::ISoundEngine *soundEngine;

    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
    static std::map<std::string, std::string> sounds;

    static std::string appendVert(const std::string &path, bool instancing = false, bool animated = false);
    static std::string appendFrag(const std::string &path, bool instancing = false, bool animated = false);
    static void setLightSources(Shader shader, glm::vec3 pointLightPositions[]);

public:
    ResourceManager(ResourceManager &other) = delete;
    void operator=(const ResourceManager &) = delete;

    static Texture setTexture(std::string textureName, std::string texturePath, bool pixelated = false);
    static Shader addShader(std::string shaderName, std::string vertexPath, std::string fragmentPath);
    static void addSound(std::string soundName, std::string soundPath);

    static Texture getTexture(std::string);
    static Shader getShader(std::string);
    static void playSound(std::string soundName, irrklang::ik_f32 volume = 1.0f, bool loop = false);

    static void initShaders(glm::vec3 pointLightPositions[]);
    static void initTextures();
};

class DuplicateResourceException : public std::exception
{
private:
    std::string name;

public:
    DuplicateResourceException(std::string name)
    {
        this->name = name;
    };

    virtual const char *what() const throw()
    {
        return strcat("There was already a resource with the name: ", name.c_str());
    }
};

class NoResourceFoundException : public std::exception
{
private:
    std::string name;

public:
    NoResourceFoundException(std::string name)
    {
        this->name = name;
    };

    virtual const char *what() const throw()
    {
        return strcat("There was already a shader with this name", name.c_str());
    }
};