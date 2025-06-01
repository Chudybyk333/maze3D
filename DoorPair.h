#pragma once
#include "DoorWing.h"
#include "Maze.h"
#include "Portal.h"
#include "Camera.h"

class DoorPair {
public:
	DoorPair();
	void Init(const glm::vec3& leftPos, const glm::vec3& rightPos);
	void LoadModel();
	void Update(float deltaTime, const glm::vec3& playerPos, bool hasKey);
	void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
	void RemoveCollidersFrom(Maze& maze);
	void RenderPortal(Shader& shader, const glm::mat4& view, const glm::mat4& proj);
	const Portal& GetPortal() const { return portal; }

private:
	DoorWing left;
	DoorWing right;
	Portal portal;
	bool opened = false;
};
