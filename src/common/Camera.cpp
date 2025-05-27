#include "Camera.h"

using namespace PoolGame3D;

Camera::Camera() {
    distance = 6.0f;
    yaw = 0.0f;
    pitch = -30.0f;
    position = glm::vec3(0.0f, 2.0f, 5.0f);
    target = glm::vec3(0.0f);
    fov = 45.0f;
}

void Camera::update() {
    // Atualiza a posição da câmera baseada em yaw e pitch
    glm::vec3 direction;
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    position = -glm::normalize(direction) * distance;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(fov), aspect, 0.5f, 100.0f);
}

void Camera::rotate(float yawDelta, float pitchDelta) {
    yaw += yawDelta;
    pitch += pitchDelta;
    
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::zoom(float amount) {
    fov -= amount;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

