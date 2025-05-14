#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include "../../stb_image.h"

class Texture
{
private:
    unsigned int textureId;

public:
    Texture(std::string texturePath, bool pixelated = false);
    Texture(const Texture &texture)
    {
        this->textureId = texture.textureId;
    }
    void bindTexture(int textureUnit = GL_TEXTURE0) const;
    unsigned int getId();
};