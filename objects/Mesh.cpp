#include "Mesh.h"

Mesh::~Mesh()
{
}

Mesh::Mesh(glm::vec3 position, glm::vec3 size, Collisioner::BoundingBoxType boundingBoxType)
    : position{position},
      size{size},
      instancing{false},
      collisioner{size, position, boundingBoxType}
{
}

Mesh::Mesh(std::vector<glm::vec3> instancePositions, glm::vec3 size, Collisioner::BoundingBoxType boundingBoxType)
    : instancing{true},
      size{size}
{
    for (auto pos : instancePositions)
    {
        this->instanceCollisioners.push_back(Collisioner(this->size, pos, boundingBoxType));
    }
}

void Mesh::draw()
{
}

std::vector<Collisioner> Mesh::isColliding(Collisioner c)
{
    // if instanced return all colliding bounding boxes
    if (this->instancing)
    {
        std::vector<Collisioner> boxes = {};
        for (auto box : this->instanceCollisioners)
        {
            if (box.isColliding(c))
            {
                boxes.push_back(box);
            }
        }
        return boxes;
    }
    // if not instanced and colliding return this bounding box
    if (this->collisioner.isColliding(c))
        ;
    return std::vector{this->collisioner};

    // else return empty vec
    return std::vector<Collisioner>();
}