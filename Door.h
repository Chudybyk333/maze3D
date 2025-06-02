#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Portal.h"
#include "Maze.h"

class Door {
public:
	Door();
	void Init(const glm::vec3& leftPos, const glm::vec3& rightPos);
	void LoadModel();
	void Update(float dt, const glm::vec3& playerPos, bool hasKey);
	void Render(Shader& shader, const glm::mat4& view, const glm::mat4& proj);
	void RenderPortal(Shader& shader, const glm::mat4& view, const glm::mat4& proj);
	void RemoveCollidersFrom(Maze& maze);
	bool IsOpen() { return isOpening; };


	const Portal& GetPortal() const;
private:
	glm::vec3 leftPosition, rightPosition;
	float leftAngle, rightAngle;
	float openSpeed;
	float maxAngle;
	float triggerDistance;
	bool isOpening;
	unsigned int VAO, VBO;
	std::vector<float> vertices;
	unsigned int textureID;

	Portal portal;

	void BuildGeometry();
	void RenderWing(Shader& shader, const glm::mat4& view, const glm::mat4& proj, const glm::vec3& basePos, float angle, bool isLeft);

};