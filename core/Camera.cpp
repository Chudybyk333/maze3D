#include "Camera.h"
#include <iostream>

Camera::Camera(
    glm::vec3 position,
    float yaw,
    float pitch) : position{position}
{
    this->setDirection(yaw, pitch);
}

glm::mat4 Camera::getView() const
{
    return glm::lookAt(position, position + direction, up);
}

void Camera::setPosition(glm::vec3 position)
{
    this->position = position;
}

void Camera::incrementPosition(glm::vec3 position)
{
    this->position += position;
}

glm::vec3 Camera::getPosition()
{
    return this->position;
}

void Camera::setDirectionByMouse(float xPos, float yPos)
{
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
    setDirection(yaw, pitch);
}

void Camera::setDirection(float yaw, float pitch)
{
    glm::vec3 directionTemp;
    directionTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    directionTemp.y = sin(glm::radians(pitch));
    directionTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->direction = glm::normalize(directionTemp);
}
