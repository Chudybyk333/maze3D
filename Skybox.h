#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 


class Skybox {
public:
    Skybox();
    ~Skybox();

    bool Load(const std::vector<std::string>& faceTextures);
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO;
    GLuint cubemapTexture;
    GLuint shaderProgram;
};

#endif