#include "maze.h"
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION

void Maze::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        map.push_back(line);
    }
    height = map.size();
    width = map.empty() ? 0 : map[0].length();
}

void Maze::SetupRender() {
    std::vector<float> vertices;

    // Załaduj teksturę
    textureID = LoadTexture("wall_texture.png");  // Upewnij się, że masz plik wall_texture.png w odpowiednim miejscu

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            if (map[z][x] == '#') {
                glm::vec3 pos(x, 0.0f, z);
                // dodaj kolizję
                glm::vec3 min(x - 0.5f, 0.0f, z - 0.5f);
                glm::vec3 max(x + 0.5f, 2.0f, z + 0.5f);
                colliders.push_back({ min, max });

                float cube[] = {
                    // pozycje         // normalne       // tekstury
                    // tył
                    -0.5f, -0.5f, -0.5f,   0, 0, -1,    0.0f, 0.0f,
                     0.5f, -0.5f, -0.5f,   0, 0, -1,    1.0f, 0.0f,
                     0.5f,  0.5f, -0.5f,   0, 0, -1,    1.0f, 1.0f,
                     0.5f,  0.5f, -0.5f,   0, 0, -1,    1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,   0, 0, -1,    0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,   0, 0, -1,    0.0f, 0.0f,

                    // przód
                    -0.5f, -0.5f,  0.5f,   0, 0, 1,    0.0f, 0.0f,
                     0.5f, -0.5f,  0.5f,   0, 0, 1,    1.0f, 0.0f,
                     0.5f,  0.5f,  0.5f,   0, 0, 1,    1.0f, 1.0f,
                     0.5f,  0.5f,  0.5f,   0, 0, 1,    1.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f,   0, 0, 1,    0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,   0, 0, 1,    0.0f, 0.0f,

                    // lewo
                    -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  -1, 0, 0,    1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  -1, 0, 0,    0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,

                    // prawo
                     0.5f,  0.5f,  0.5f,   1, 0, 0,    1.0f, 0.0f,
                     0.5f,  0.5f, -0.5f,   1, 0, 0,    1.0f, 1.0f,
                     0.5f, -0.5f, -0.5f,   1, 0, 0,    0.0f, 1.0f,
                     0.5f, -0.5f, -0.5f,   1, 0, 0,    0.0f, 1.0f,
                     0.5f, -0.5f,  0.5f,   1, 0, 0,    0.0f, 0.0f,
                     0.5f,  0.5f,  0.5f,   1, 0, 0,    1.0f, 0.0f,

                     // dół
                     -0.5f, -0.5f, -0.5f,   0, -1, 0,    0.0f, 1.0f,
                      0.5f, -0.5f, -0.5f,   0, -1, 0,    1.0f, 1.0f,
                      0.5f, -0.5f,  0.5f,   0, -1, 0,    1.0f, 0.0f,
                      0.5f, -0.5f,  0.5f,   0, -1, 0,    1.0f, 0.0f,
                     -0.5f, -0.5f,  0.5f,   0, -1, 0,    0.0f, 0.0f,
                     -0.5f, -0.5f, -0.5f,   0, -1, 0,    0.0f, 1.0f,

                     // góra
                     -0.5f,  0.5f, -0.5f,   0, 1, 0,    0.0f, 1.0f,
                      0.5f,  0.5f, -0.5f,   0, 1, 0,    1.0f, 1.0f,
                      0.5f,  0.5f,  0.5f,   0, 1, 0,    1.0f, 0.0f,
                      0.5f,  0.5f,  0.5f,   0, 1, 0,    1.0f, 0.0f,
                     -0.5f,  0.5f,  0.5f,   0, 1, 0,    0.0f, 0.0f,
                     -0.5f,  0.5f, -0.5f,   0, 1, 0,    0.0f, 1.0f,
                };

                // przesunięcie wszystkich wierzchołków
                for (int i = 0; i < 36; ++i) {
                    int idx = i * 8;
                    vertices.push_back(cube[idx + 0] + pos.x);
                    vertices.push_back(cube[idx + 1] + pos.y + 0.5f);
                    vertices.push_back(cube[idx + 2] + pos.z);
                    vertices.push_back(cube[idx + 3]);
                    vertices.push_back(cube[idx + 4]);
                    vertices.push_back(cube[idx + 5]);
                    vertices.push_back(cube[idx + 6]);
                    vertices.push_back(cube[idx + 7]);
                }
            }
        }
    }

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    vertexCount = vertices.size() / 8;
}

void Maze::Render(Shader& shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("texture_diffuse1", 0);

    glBindVertexArray(cubeVAO);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

const std::vector<AABB>& Maze::GetColliders() const {
    return colliders;
}
