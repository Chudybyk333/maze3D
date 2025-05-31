#include "camera.h"

Camera::Camera()
    : Front(0.0f, 0.0f, -1.0f), Speed(1.0f), Sensitivity(0.1f),
    firstMouse(true), lastX(400), lastY(300), verticalVelocity(0.0f) { // Initialize verticalVelocity
    Position = glm::vec3(12.0f, groundHeight, 12.0f); // ok. „wzrost gracza”
    prevPosition = Position;
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f;
    Pitch = 0.0f;
	Fov = 80.0f; // standardowy FOV
    sprintFov = 90.0f;   
    currentFov = 80.0f;   
    fovChangeSpeed = 10.0f;
}


bool Camera::IsMoving() const {
    glm::vec3 diff = Position - prevPosition;
    diff.y = 0.0f; // pomijamy ruch w pionie
    return glm::length(diff) > 0.001f; // próg detekcji ruchu
}

void Camera::UpdatePrevPosition() {
    prevPosition = Position;
}


bool Camera::CheckCollision(const glm::vec3& newPos, float radius) const {
    if (!maze) return false;  // brak labiryntu = brak kolizji

    for (const auto& box : maze->GetColliders()) {
        glm::vec3 clamped = glm::clamp(newPos, box.min, box.max);
        float dist = glm::distance(newPos, clamped);
        if (dist < radius)
            return true;
    }
    for (const auto& box : maze->GetDoorColliders()) {
        glm::vec3 clamped = glm::clamp(newPos, box.min, box.max);
        float dist = glm::distance(newPos, clamped);
        if (dist < radius)
            return true;
    }
    return false;
}

void Camera::Update() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Większy FOV podczas sprintu z efektem "przyśpieszenia"
    float targetFov = isSprinting ? sprintFov : Fov;
    if (isSprinting) {
        targetFov += 5.0f * sin(glfwGetTime() * 1.0f); // Lekkie drgania
    }
    currentFov = glm::mix(currentFov, targetFov, fovChangeSpeed * 0.016f);
}
void Camera::ProcessKeyboard(GLFWwindow* window) {
    float velocity = GetCurrentSpeed() * 0.016f; // 0.016f to przybliżony czas jednej klatki (60 FPS)
    glm::vec3 moveDir(0.0f);
    static bool altPressedLastFrame = false;

    // Sprint
    isSprinting = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir += Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir -= Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir -= glm::normalize(glm::cross(Front, Up));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir += glm::normalize(glm::cross(Front, Up));
    if (glm::length(moveDir) > 0.0f) {
        moveDir.y = 0.0f;
        moveDir = glm::normalize(moveDir) * velocity;
        // Osobno oś X
        glm::vec3 proposedX = Position + glm::vec3(moveDir.x, 0.0f, 0.0f);
        if (!CheckCollision(proposedX, hitboxRadius)) {
            Position.x = proposedX.x;
        }

        // Osobno oś Z
        glm::vec3 proposedZ = Position + glm::vec3(0.0f, 0.0f, moveDir.z);
        if (!CheckCollision(proposedZ, hitboxRadius)) {
            Position.z = proposedZ.z;
        }
    }

    if (flyMode) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Position.y += velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            Position.y -= velocity;
    }
    else {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
            verticalVelocity = jumpForce;
            isGrounded = false;
        }
    }

    bool altPressedNow = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
    if (altPressedNow && !altPressedLastFrame) {
        flyMode = !flyMode;  // przełącz tryb latania
    }
    altPressedLastFrame = altPressedNow;

}

void Camera::UpdatePhysics(float deltaTime) {
    if (!flyMode) {
        verticalVelocity += gravity * deltaTime * 0.5f;
        Position.y += verticalVelocity * deltaTime * 0.5f;

        if (CheckCollision(Position, hitboxRadius)) {
            Position.y -= verticalVelocity * deltaTime;
            verticalVelocity = 0.0f;
            isGrounded = true;
        }
        if (Position.y <= groundHeight) {
            Position.y = groundHeight;
            verticalVelocity = 0.0f;
            isGrounded = true;
        }
    }
}



void Camera::OnMouseMove(GLFWwindow* window, double xpos, double ypos) {
    // Pobierz wymiary okna
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Oblicz środek okna
    const double centerX = width / 2.0;
    const double centerY = height / 2.0;

    // Jeśli to pierwszy ruch myszy, ustaw kursory na środek
    if (firstMouse) {
        lastX = centerX;
        lastY = centerY;
        glfwSetCursorPos(window, centerX, centerY);  // Ustaw kursor na środek
        firstMouse = false;
        return;
    }

    // Oblicz przesunięcie myszy
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Ogranicz Pitch
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    Update();

    // Przenieś kursor na środek
    glfwSetCursorPos(window, centerX, centerY);
    lastX = centerX;
    lastY = centerY;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(currentFov), 800.0f / 600.0f, 0.1f, 100.0f);
}
