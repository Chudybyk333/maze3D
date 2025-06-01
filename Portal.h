#pragma once
#include <glm/glm.hpp>
#include "shader.h"

class Portal {
public:
	Portal();
	void Load();
	void SetPosition(const glm::vec3& pos);
	void Show();
	void Hide();
	void Update(float deltaTime);
	void Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
	inline glm::vec3 GetPosition() const { return position; }
	inline float GetIntensity() const { return intensity; }
	inline bool IsVisible() const { return visible; }

private:
	glm::vec3 position;
	bool visible;
	float intensity;
	float maxIntensity;
	float growSpeed;
	unsigned int VAO, VBO, textureID;
};