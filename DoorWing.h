#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include <vector>

class DoorWing {
public:
	DoorWing(bool isLeft);
	void LoadModel();
	void SetPosition(const glm::vec3& pos);
	void Update(float deltaTime, const glm::vec3& playerPos, bool hasKey);
	void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
	glm::vec3 GetPosition() const;

private:
	unsigned int VAO, VBO, textureID;
	std::vector<float> vertices;
	glm::vec3 basePosition;
	float rotationAngle;
	bool opening;
	bool isLeft;
	float maxAngle;
	float triggerDistance;
	float openSpeed;
};