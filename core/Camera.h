#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    float sensitivity = 0.05f;

    glm::vec3 position; // position of camera

    float yaw;   // degrees around the y-axis
    float pitch; // degrees around the x-axis

    // screen coordinates
    float lastX = 0.0f;
    float lastY = 0.0f;

    void setDirection(float yaw, float pitch);

protected:
    void setPosition(glm::vec3 position);
    void incrementPosition(glm::vec3 position);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //  normalized up-direction
    glm::vec3 direction;                        // normalized direction the camera is pointing to
public:
    Camera(
        glm::vec3 position = glm::vec3{},
        float yaw = -90.0f,
        float pitch = 0.0f);

    glm::mat4 getView() const;

    void setDirectionByMouse(float xPos, float yPos);
    glm::vec3 getPosition();
};