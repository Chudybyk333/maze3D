#include "DoorWing.h"
#include "texture.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

DoorWing::DoorWing(bool isLeftWing)
	: VAO(0), VBO(0), textureID(0), basePosition(0.0f), rotationAngle(0.0f),
	opening(false), isLeft(isLeftWing), maxAngle(90.0f),
	triggerDistance(2.0f), openSpeed(60.0f) {}

glm::vec3 DoorWing::GetPosition() const {
	return basePosition;
}

void DoorWing::SetPosition(const glm::vec3& pos) {
	basePosition = pos;
}

void DoorWing::LoadModel() {
    float w = 1.0f;
    float h = 2.0f;
    float d = 0.2f;
    float hw = w / 2.0f;
    float hh = h / 2.0f;
    float hd = d / 2.0f;

    float cube[] = {
        // ty³
        -hw, -hh, -hd,  0, 0, -1,  0.0f, 0.0f,
         hw, -hh, -hd,  0, 0, -1,  1.0f, 0.0f,
         hw,  hh, -hd,  0, 0, -1,  1.0f, 1.0f,
         hw,  hh, -hd,  0, 0, -1,  1.0f, 1.0f,
        -hw,  hh, -hd,  0, 0, -1,  0.0f, 1.0f,
        -hw, -hh, -hd,  0, 0, -1,  0.0f, 0.0f,

        // przód
        -hw, -hh,  hd,  0, 0, 1,  0.0f, 0.0f,
         hw, -hh,  hd,  0, 0, 1,  1.0f, 0.0f,
         hw,  hh,  hd,  0, 0, 1,  1.0f, 1.0f,
         hw,  hh,  hd,  0, 0, 1,  1.0f, 1.0f,
        -hw,  hh,  hd,  0, 0, 1,  0.0f, 1.0f,
        -hw, -hh,  hd,  0, 0, 1,  0.0f, 0.0f,

        // lewa
        -hw,  hh,  hd, -1, 0, 0,  1.0f, 0.0f,
        -hw,  hh, -hd, -1, 0, 0,  1.0f, 1.0f,
        -hw, -hh, -hd, -1, 0, 0,  0.0f, 1.0f,
        -hw, -hh, -hd, -1, 0, 0,  0.0f, 1.0f,
        -hw, -hh,  hd, -1, 0, 0,  0.0f, 0.0f,
        -hw,  hh,  hd, -1, 0, 0,  1.0f, 0.0f,

        // prawa
         hw,  hh,  hd,  1, 0, 0,  1.0f, 0.0f,
         hw,  hh, -hd,  1, 0, 0,  1.0f, 1.0f,
         hw, -hh, -hd,  1, 0, 0,  0.0f, 1.0f,
         hw, -hh, -hd,  1, 0, 0,  0.0f, 1.0f,
         hw, -hh,  hd,  1, 0, 0,  0.0f, 0.0f,
         hw,  hh,  hd,  1, 0, 0,  1.0f, 0.0f,

         // dó³
         -hw, -hh, -hd, 0, -1, 0, 0.0f, 1.0f,
          hw, -hh, -hd, 0, -1, 0, 1.0f, 1.0f,
          hw, -hh,  hd, 0, -1, 0, 1.0f, 0.0f,
          hw, -hh,  hd, 0, -1, 0, 1.0f, 0.0f,
         -hw, -hh,  hd, 0, -1, 0, 0.0f, 0.0f,
         -hw, -hh, -hd, 0, -1, 0, 0.0f, 1.0f,

         // góra
         -hw,  hh, -hd, 0, 1, 0, 0.0f, 1.0f,
          hw,  hh, -hd, 0, 1, 0, 1.0f, 1.0f,
          hw,  hh,  hd, 0, 1, 0, 1.0f, 0.0f,
          hw,  hh,  hd, 0, 1, 0, 1.0f, 0.0f,
         -hw,  hh,  hd, 0, 1, 0, 0.0f, 0.0f,
         -hw,  hh, -hd, 0, 1, 0, 0.0f, 1.0f,
    };

    vertices.assign(cube, cube + sizeof(cube) / sizeof(float));
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
void DoorWing::Update(float dt, const glm::vec3& playerPos, bool hasKey) {
    if (!opening && hasKey && glm::distance(playerPos, basePosition) < triggerDistance)
        opening = true;
    if (opening && std::abs(rotationAngle) < maxAngle) {
        float dir = isLeft ? -1.0f : 1.0f;
        rotationAngle += dir * openSpeed * dt;
        rotationAngle = glm::clamp(rotationAngle, -maxAngle, maxAngle);
    }
}
void DoorWing::Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);

    glm::vec3 pivotOffset = glm::vec3((isLeft ? -0.5f : 0.5f), 0.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), basePosition + pivotOffset);
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0, 1, 0));
    model = glm::translate(model, -pivotOffset);

    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
    glBindVertexArray(0);
}