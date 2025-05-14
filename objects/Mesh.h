
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iterator>
#include <iostream>

#include "Collisioner.h"

class Mesh
{
protected:
    glm::vec3 position, size;
    // glm::vec3   color;
    // float       rotation;
    Collisioner collisioner;

    bool instancing;
    std::vector<glm::mat4> instanceModelMatrices;
    std::vector<Collisioner> instanceCollisioners;

public:
    virtual ~Mesh();
    Mesh(
        glm::vec3 position,
        glm::vec3 size = glm::vec3{1.0f},
        Collisioner::BoundingBoxType boundingBoxType = Collisioner::BoundingBoxType::NONE);
    // instancing constructor
    Mesh(
        std::vector<glm::vec3> instancePositions,
        glm::vec3 size,
        Collisioner::BoundingBoxType boundingBoxType = Collisioner::BoundingBoxType::NONE);
    // void rotate(float angleInDegrees) { this->rotation += angleInDegrees; };
    void move(glm::vec3 pos) { this->position += pos; };
    // void setSize (glm::vec3 size) { this->size = size; };
    // void setColor (glm::vec3 col) { this->color = col; };
    // void setVertices(float vertices[]) { this->vertices.insert(this->vertices.end(), vertices, vertices + sizeof(vertices)/sizeof(float)); };
    // void addVertex(float vertex) { this->vertices.push_back(vertex); }
    virtual void draw();
    std::vector<Collisioner> isColliding(Collisioner c);
};