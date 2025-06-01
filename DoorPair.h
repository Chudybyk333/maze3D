#pragma once
#include "DoorWing.h"
#include "Maze.h"

class DoorPair {
public:
	DoorPair();
	void Init(const glm::vec3& leftPos, const glm::vec3& rightPos);
	void LoadModel();
	void Update(float deltaTime, const glm::vec3& playerPos, bool hasKey);
	void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
	void RemoveCollidersFrom(Maze& maze);

private:
	DoorWing left;
	DoorWing right;
	bool opened;
};
