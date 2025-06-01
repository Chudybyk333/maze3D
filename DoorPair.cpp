#include "DoorPair.h"

DoorPair::DoorPair() : opened(false), left(true), right(false) {}

void DoorPair::Init(const glm::vec3& leftPos, const glm::vec3& rightPos) {
	left.SetPosition(leftPos);
	right.SetPosition(rightPos);
}

void DoorPair::LoadModel() {
	left.LoadModel();
	right.LoadModel();
}

void DoorPair::Update(float dt, const glm::vec3& playerPos, bool hasKey) {
	glm::vec3 center = (left.GetPosition() + right.GetPosition()) * 0.5f;
	float dist = glm::distance(playerPos, center);
	if (!opened && hasKey && dist < 2.0f) {
		opened = true;
	}

	if (opened) {
		left.Update(dt, playerPos, true);
		right.Update(dt, playerPos, true);
	}
}

void DoorPair::Render(Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
	left.Render(shader, view, proj);
	right.Render(shader, view, proj);
}

void DoorPair::RemoveCollidersFrom(Maze& maze) {
	maze.RemoveDoorColliderAt(left.GetPosition());
	maze.RemoveDoorColliderAt(right.GetPosition());
}