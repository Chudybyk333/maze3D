#include "Crosshair.h"

Crosshair::Crosshair(float width, float height)
    : shader{ResourceManager::getShader("default2D")}
{
    vertices = {
        -width, 0,
        -width / 3, 0,
        width, 0,
        width / 3, 0,
        0, -height,
        0, -height / 3,
        0, height,
        0, height / 3};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &(vertices.front()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

Crosshair::~Crosshair()
{
}

void Crosshair::draw()
{
    glDisable(GL_DEPTH_TEST);
    shader.use();

    glLineWidth(2.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 8);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}