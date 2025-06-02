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
    float walkSpeed = 1.6f;
    float sprintSpeed = 2.4f;
    bool isSprinting = false;
    void SetFov(float fov) { currentFov = fov; }
    float GetFov() const { return currentFov; }

private:
    glm::vec3 Position, Front, Up;
    glm::vec3 prevPosition;
    
    // Widok
    float Yaw, Pitch, Speed, Sensitivity;
    float Fov, sprintFov, currentFov, fovChangeSpeed;

    bool firstMouse;
    float lastX, lastY;
    float verticalVelocity;
    bool isGrounded = true;

	// Parametry fizyki
    const float groundHeight = 1.5f; 
    const float gravity = -3.0f;
    const float jumpForce = 2.5f;

    float hitboxRadius = 0.15f;
    bool flyMode = false;
    float GetCurrentSpeed() const { return isSprinting ? sprintSpeed : walkSpeed; }
    bool CheckCollision(const glm::vec3& newPos, float radius) const;
};