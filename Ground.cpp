#include "Ground.h"
#include "texture.h"
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include "Maze.h"

Ground::Ground() : groundVAO(0), groundVBO(0), textureID(0), vertexCount(0) {}

void Ground::SetupRender(const Maze& maze) {
    // £adowanie struktury pod³owgi
    textureID = LoadTexture("ground_texture.png");

    // Ustaw parametry tekstury dla tilingu
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Pobranie rozmiaru labiryntu
    int mazeWidth = maze.GetWidth();
    int mazeHeight = maze.GetHeight();

    // Oblicz rozmiar p³aszczyzny
    float startX = - 0.5f;
    float startZ = - 0.5f;
    float endX = mazeWidth - 0.5f;
    float endZ = mazeHeight - 0.5f;

    // powtarzalnoœæ tekstury
    float texRepeatX = (endX - startX) / 1.0f;
    float texRepeatZ = (endZ - startZ) / 1.0f;

    std::vector<float> vertices = {
        // pozycje          // normalne   // tekstury
        startX, 0.0f, startZ,  0.0f, 1.0f, 0.0f,  0.0f, texRepeatZ,
        endX, 0.0f, startZ,  0.0f, 1.0f, 0.0f,  texRepeatX, texRepeatZ,
        endX, 0.0f, endZ,  0.0f, 1.0f, 0.0f,  texRepeatX, 0.0f,

        endX, 0.0f, endZ,  0.0f, 1.0f, 0.0f,  texRepeatX, 0.0f,
        startX, 0.0f, endZ,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        startX, 0.0f, startZ,  0.0f, 1.0f, 0.0f,  0.0f, texRepeatZ
    };
    vertexCount = vertices.size() / 8;

    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Ground::Render(Shader& shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("texture_diffuse1", 0);

    glBindVertexArray(groundVAO);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}