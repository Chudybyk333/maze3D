#include "Door.h"
#include "texture.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Door::Door()
	: leftAngle(0), rightAngle(0), openSpeed(60.0f), maxAngle(90.0f),
	triggerDistance(2.0f), isOpening(false), VAO(0), VBO(0), textureID(0) {}

void Door::Init(const glm::vec3& leftPos, const glm::vec3& rightPos) {
	leftPosition = leftPos;
	rightPosition = rightPos;
	glm::vec3 center = (leftPos + rightPos) * 0.5f;
	portal.SetPosition(center + glm::vec3(0, 0, 0.6f));
	portal.Load();
}

void Door::BuildGeometry() {
    float w = 1.0f, h = 2.0f, d = 0.2f;
    float hw = w / 2.0f, hh = h / 2.0f, hd = d / 2.0f;
    float cube[] = {
        // back
        -hw, -hh, -hd,  0, 0, -1,  0.0f, 0.0f,
         hw, -hh, -hd,  0, 0, -1,  1.0f, 0.0f,
         hw,  hh, -hd,  0, 0, -1,  1.0f, 1.0f,
         hw,  hh, -hd,  0, 0, -1,  1.0f, 1.0f,
        -hw,  hh, -hd,  0, 0, -1,  0.0f, 1.0f,
        -hw, -hh, -hd,  0, 0, -1,  0.0f, 0.0f,

        // front
        -hw, -hh,  hd,  0, 0, 1,  0.0f, 0.0f,
         hw, -hh,  hd,  0, 0, 1,  1.0f, 0.0f,
         hw,  hh,  hd,  0, 0, 1,  1.0f, 1.0f,
         hw,  hh,  hd,  0, 0, 1,  1.0f, 1.0f,
        -hw,  hh,  hd,  0, 0, 1,  0.0f, 1.0f,
        -hw, -hh,  hd,  0, 0, 1,  0.0f, 0.0f,

        // left
        -hw,  hh,  hd, -1, 0, 0,  1.0f, 0.0f,
        -hw,  hh, -hd, -1, 0, 0,  1.0f, 1.0f,
        -hw, -hh, -hd, -1, 0, 0,  0.0f, 1.0f,
        -hw, -hh, -hd, -1, 0, 0,  0.0f, 1.0f,
        -hw, -hh,  hd, -1, 0, 0,  0.0f, 0.0f,
        -hw,  hh,  hd, -1, 0, 0,  1.0f, 0.0f,

        // right
         hw,  hh,  hd,  1, 0, 0,  1.0f, 0.0f,
         hw,  hh, -hd,  1, 0, 0,  1.0f, 1.0f,
         hw, -hh, -hd,  1, 0, 0,  0.0f, 1.0f,
         hw, -hh, -hd,  1, 0, 0,  0.0f, 1.0f,
         hw, -hh,  hd,  1, 0, 0,  0.0f, 0.0f,
         hw,  hh,  hd,  1, 0, 0,  1.0f, 0.0f,

         // bottom
         -hw, -hh, -hd,  0, -1, 0,  0.0f, 1.0f,
          hw, -hh, -hd,  0, -1, 0,  1.0f, 1.0f,
          hw, -hh,  hd,  0, -1, 0,  1.0f, 0.0f,
          hw, -hh,  hd,  0, -1, 0,  1.0f, 0.0f,
         -hw, -hh,  hd,  0, -1, 0,  0.0f, 0.0f,
         -hw, -hh, -hd,  0, -1, 0,  0.0f, 1.0f,

         // top
         -hw,  hh, -hd,  0, 1, 0,  0.0f, 1.0f,
          hw,  hh, -hd,  0, 1, 0,  1.0f, 1.0f,
          hw,  hh,  hd,  0, 1, 0,  1.0f, 0.0f,
          hw,  hh,  hd,  0, 1, 0,  1.0f, 0.0f,
         -hw,  hh,  hd,  0, 1, 0,  0.0f, 0.0f,
         -hw,  hh, -hd,  0, 1, 0,  0.0f, 1.0f,
    };

    vertices.assign(cube, cube + sizeof(cube) / sizeof(float));
}
void Door::LoadModel() {
    BuildGeometry();
    textureID = LoadTexture("door.png");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
void Door::Update(float dt, const glm::vec3& playerPos, bool hasKey) {
    glm::vec3 center = (leftPosition + rightPosition) * 0.5f;
    float dist = glm::distance(center, playerPos);
    if (!isOpening && hasKey && dist < triggerDistance) {
        isOpening = true;
        portal.Show();
    }
    if (isOpening) {
        float delta = openSpeed * dt;
        leftAngle = glm::clamp(leftAngle - delta, -maxAngle, 0.0f);
        rightAngle = glm::clamp(rightAngle + delta, 0.0f, maxAngle);
        portal.Update(dt);
    }
}
void Door::Render(Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    RenderWing(shader, view, proj, leftPosition, leftAngle, true);
    RenderWing(shader, view, proj, rightPosition, rightAngle, false);
}

void Door::RenderWing(Shader& shader, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& basePos, float angle, bool isLeft) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", proj);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glm::vec3 pivotOffset = glm::vec3((isLeft ? -0.5f : 0.5f), 0.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), basePos + pivotOffset);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    model = glm::translate(model, -pivotOffset);

    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
    glBindVertexArray(0);
}
void Door::RenderPortal(Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
    portal.Render(shader, view, proj);
}

void Door::RemoveCollidersFrom(Maze& maze) {
    maze.RemoveDoorColliderAt(leftPosition);
    maze.RemoveDoorColliderAt(rightPosition);
}

const Portal& Door::GetPortal() const {
    return portal;
}