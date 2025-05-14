#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(int amount, Texture texture)
    : amount{amount},
      texture{texture},
      shader{ResourceManager::getShader("particle")}
{
    for (int i = 0; i < amount; i++)
    {
        particles.push_back(Particle());
    }
    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f, 0.5f, 0.5f, // 1
        0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 2
        -0.5f, 0.5f, 0.0f, 0.0f, 0.5f
    };

    std::vector<int> indices = {
        3, 1, 0,
        3, 2, 1
    };

    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void ParticleGenerator::addParticles(float dt, glm::vec3 position)
{
    timePassedSincePrevSpawn += dt;
    if (timePassedSincePrevSpawn >= particleSpawnDelay)
    {
        timePassedSincePrevSpawn = 0.0f;
        int amountNewParticles = 1;
        for (int i = 0; i < amountNewParticles; i++)
        {
            int deadParticleIndex = getFirstDeadParticleIndex();
            if (deadParticleIndex != -1)
            {
                particles[deadParticleIndex].respawn(position);
            }
        }
    }
}

void ParticleGenerator::update(float dt)
{
    for (auto &particle : particles)
    {
        particle.update(dt);
    }
}

void ParticleGenerator::draw()
{
    shader.use();
    texture.bindTexture();
    for (auto particle : particles)
    {
        if (particle.isAlive())
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, particle.getPosition());
            model = glm::scale(model, particle.getSize());
            shader.setMatrixFloat4("model", model);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}

int ParticleGenerator::getFirstDeadParticleIndex()
{
    for (int i = lastUsedParticleIndex; i < amount; i++)
    {
        if (!this->particles[i].isAlive())
        {
            return i;
        }
    }
    for (int i = 0; i < lastUsedParticleIndex; i++)
    {
        if (!this->particles[i].isAlive())
        {
            return i;
        }
    }
    return -1;
}
