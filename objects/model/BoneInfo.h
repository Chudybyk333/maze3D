#pragma once

#include <glm/glm.hpp>

struct BoneInfo
{
	int id;

	// transformation model-space -> bone-space
	glm::mat4 offset;
};