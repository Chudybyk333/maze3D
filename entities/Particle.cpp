#include "Particle.h"

Particle::Particle()
{
}

bool Particle::isAlive()
{
    return life > 0.0f;
}

void Particle::update(float dt)
{
    if (this->isAlive())
    {
        life -= dt;
        position += velocity * dt;
    }
}

void Particle::respawn(glm::vec3 position)
{
    life = initialLife;
    float randomX = ((rand() % 100) - 50) / 1000.0f;
    float randomY = ((rand() % 100)) / 1000.0f;
    float randomZ = ((rand() % 100) - 50) / 1000.0f;
    position.x += randomX;
    position.y += randomY;
    position.z += randomZ;
    velocity.x = randomX / 10.0f;
    velocity.z = randomZ / 10.0f;
    this->position = position;
}

glm::vec3 Particle::getPosition()
{
    return this->position;
}

glm::vec3 Particle::getSize()
{
    return this->size;
}