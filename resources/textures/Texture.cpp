#include "Texture.h"

Texture::Texture(std::string texturePath, bool pixelated)
{
    int width, height, nrChannels;
    unsigned char *data;

    glGenTextures(1, &this->textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // set the texture wrapping/filtering options
    // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (pixelated)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    std::string suffix = ".jpg";
    int colorChannels;
    // check if path ends with .png, then we can use RGBA
    if (texturePath.size() >= suffix.size() && 0 == texturePath.compare(texturePath.size() - suffix.size(), suffix.size(), suffix))
    {
        colorChannels = GL_RGB;
    }
    else
    {
        colorChannels = GL_RGBA;
    }
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, colorChannels, width, height, 0, colorChannels, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }

    stbi_image_free(data); // free image
}

void Texture::bindTexture(int textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

unsigned int Texture::getId()
{
    return textureId;
}
