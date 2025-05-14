#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
private:
    glm::vec3 Position, Front, Up;
    float Yaw, Pitch, Speed, Sensitivity, Fov;
    bool firstMouse;
    float lastX, lastY;
    float verticalVelocity;
    bool isGrounded = true;
    const float groundHeight = 0.5f;  // lub inna wartość, np. 1.2f
    const float gravity = -9.81f;
    const float jumpForce = 5.0f;
    bool flyMode = false;
};