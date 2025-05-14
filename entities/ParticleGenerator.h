#pragma once
#include <vector>
#include "Particle.h"

class ParticleGenerator
{
private:
    std::vector<Particle> particles{};
    int amount = 1;
    int lastUsedParticleIndex = 0;

    float timePassedSincePrevSpawn = 0.0f;
    float particleSpawnDelay = 0.01f;

    unsigned int VAO;
    unsigned int EBO;

    Shader shader;
    Texture texture;

    int getFirstDeadParticleIndex();

public:
    ParticleGenerator(
        int amount,
        Texture texture);

    void update(float dt);
    void addParticles(float dt, glm::vec3 position);
    void draw();
};