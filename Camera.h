#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "maze.h"
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera();
    void Update();
    void ProcessKeyboard(GLFWwindow* window);
    void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
    void UpdatePhysics(float deltaTime);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
	glm::vec3 GetPosition() const { return Position; }
    const Maze* maze = nullptr;
    void SetMaze(const Maze* m) { maze = m; }
    bool IsMoving() const;
    bool IsJumping() const { return isGrounded; }
    void UpdatePrevPosition();
private:
    glm::vec3 Position, Front, Up;
    glm::vec3 prevPosition;
    float Yaw, Pitch, Speed, Sensitivity, Fov;
    bool firstMouse;
    float lastX, lastY;
    float verticalVelocity;
    bool isGrounded = true;
    const float groundHeight = 0.5f;  // lub inna wartość, np. 1.2f
    const float gravity = -9.81f;
    const float jumpForce = 3.5f;
    float hitboxRadius = 0.15f;
    bool flyMode = false;
    bool CheckCollision(const glm::vec3& newPos, float radius) const;
};