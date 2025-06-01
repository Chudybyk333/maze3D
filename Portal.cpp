#include "Portal.h"
#include <glad/glad.h>
#include "texture.h"
#include <glm/gtc/matrix_transform.hpp>

Portal::Portal()
	: visible(false), intensity(0.0f), maxIntensity(3.0f), growSpeed(1.5f) {}

void Portal::SetPosition(const glm::vec3& pos) {
	position = pos - glm::vec3(0.0f, 1.0f, 0.0f); // przesuwamy w dó³ o po³owê wysokoœci
}

void Portal::Show() {
	visible = true;
}

void Portal::Hide() {
	visible = false;
	intensity = 0.0f;
}

void Portal::Load() {
	float size = 1.0f;
	float vertices[] = {
		// positions        // normals       // tex coords
		-size, 0.0f, 0.0f,   0, 0, 1,   0.0f, 0.0f,
		 size, 0.0f, 0.0f,   0, 0, 1,   1.0f, 0.0f,
		 size, 2.0f, 0.0f,   0, 0, 1,   1.0f, 1.0f,
		 size, 2.0f, 0.0f,   0, 0, 1,   1.0f, 1.0f,
		-size, 2.0f, 0.0f,   0, 0, 1,   0.0f, 1.0f,
		-size, 0.0f, 0.0f,   0, 0, 1,   0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	textureID = LoadTexture("portal.png");  // mo¿esz u¿yæ pustej lub œwiec¹cej tekstury
}

void Portal::Update(float dt) {
	if (!visible) return;
	if (intensity < maxIntensity)
		intensity += growSpeed * dt;
		if (intensity > maxIntensity)
			intensity = maxIntensity;
}

void Portal::Render(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
	if (!visible) return;
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", glm::translate(glm::mat4(1.0f), position));
	shader.setFloat("emissiveStrength", intensity);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.setInt("texture_diffuse1", 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}