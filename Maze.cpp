#include "maze.h"
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            if (map[z][x] == '#') {
                // generuj wierzchołki dla sześcianu w (x,z)
                glm::vec3 pos(x, 0.0f, z);

                float cube[] = {
                    // pozycje         // normalne
                    // tył
                    -0.5f, -0.5f, -0.5f,   0, 0, -1,
                     0.5f, -0.5f, -0.5f,   0, 0, -1,
                     0.5f,  0.5f, -0.5f,   0, 0, -1,
                     0.5f,  0.5f, -0.5f,   0, 0, -1,
                    -0.5f,  0.5f, -0.5f,   0, 0, -1,
                    -0.5f, -0.5f, -0.5f,   0, 0, -1,

                    // przód
                    -0.5f, -0.5f,  0.5f,   0, 0, 1,
                     0.5f, -0.5f,  0.5f,   0, 0, 1,
                     0.5f,  0.5f,  0.5f,   0, 0, 1,
                     0.5f,  0.5f,  0.5f,   0, 0, 1,
                    -0.5f,  0.5f,  0.5f,   0, 0, 1,
                    -0.5f, -0.5f,  0.5f,   0, 0, 1,

                    // lewo
                    -0.5f,  0.5f,  0.5f,  -1, 0, 0,
                    -0.5f,  0.5f, -0.5f,  -1, 0, 0,
                    -0.5f, -0.5f, -0.5f,  -1, 0, 0,
                    -0.5f, -0.5f, -0.5f,  -1, 0, 0,
                    -0.5f, -0.5f,  0.5f,  -1, 0, 0,
                    -0.5f,  0.5f,  0.5f,  -1, 0, 0,

                    // prawo
                     0.5f,  0.5f,  0.5f,   1, 0, 0,
                     0.5f,  0.5f, -0.5f,   1, 0, 0,
                     0.5f, -0.5f, -0.5f,   1, 0, 0,
                     0.5f, -0.5f, -0.5f,   1, 0, 0,
                     0.5f, -0.5f,  0.5f,   1, 0, 0,
                     0.5f,  0.5f,  0.5f,   1, 0, 0,

                     // dół
                     -0.5f, -0.5f, -0.5f,   0, -1, 0,
                      0.5f, -0.5f, -0.5f,   0, -1, 0,
                      0.5f, -0.5f,  0.5f,   0, -1, 0,
                      0.5f, -0.5f,  0.5f,   0, -1, 0,
                     -0.5f, -0.5f,  0.5f,   0, -1, 0,
                     -0.5f, -0.5f, -0.5f,   0, -1, 0,

                     // góra
                     -0.5f,  0.5f, -0.5f,   0, 1, 0,
                      0.5f,  0.5f, -0.5f,   0, 1, 0,
                      0.5f,  0.5f,  0.5f,   0, 1, 0,
                      0.5f,  0.5f,  0.5f,   0, 1, 0,
                     -0.5f,  0.5f,  0.5f,   0, 1, 0,
                     -0.5f,  0.5f, -0.5f,   0, 1, 0,
                };

                // przesunięcie wszystkich wierzchołków
                for (int i = 0; i < 36; ++i) {
                    int idx = i * 6;
                    vertices.push_back(cube[idx + 0] + pos.x);
                    vertices.push_back(cube[idx + 1] + pos.y + 0.5f); // środek w Y
                    vertices.push_back(cube[idx + 2] + pos.z);
                    vertices.push_back(cube[idx + 3]);
                    vertices.push_back(cube[idx + 4]);
                    vertices.push_back(cube[idx + 5]);
                }
            }
        }
    }

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    vertexCount = vertices.size() / 6;
}

void Maze::Render(Shader& shader) {
    glBindVertexArray(cubeVAO);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
