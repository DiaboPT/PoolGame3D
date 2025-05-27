#include <GL/glew.h>  // GLEW deve vir primeiro
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "App.h"
#include <iostream>

using namespace PoolGame3D;

InputManager::InputManager() : window(nullptr), camera(nullptr), app(nullptr), isRotating(false), lastMouseX(0.0), lastMouseY(0.0) {}

InputManager::~InputManager() {}

void InputManager::initialize(GLFWwindow* win) {
    window = win;
    glfwSetWindowUserPointer(window, this);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                self->isRotating = true;
                glfwGetCursorPos(w, &self->lastMouseX, &self->lastMouseY);
            } else if (action == GLFW_RELEASE) {
                self->isRotating = false;
            }
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
        if (!self || !self->isRotating) return;
        float sensitivity = 0.1f;
        float xoffset = xpos - self->lastMouseX;
        float yoffset = self->lastMouseY - ypos;
        self->lastMouseX = xpos;
        self->lastMouseY = ypos;
        if (self->camera) {
            self->camera->rotate(xoffset * sensitivity, yoffset * sensitivity);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        if (self->camera) {
            self->camera->zoom((float)yoffset);
        }
    });

    // Key callback orientado a objeto
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        auto* self = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        self->keyCallback(w, key, scancode, action, mods);
    });
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Sempre desativa todas as luzes antes de ativar a escolhida
        app->ambientLight.enabled = false;
        app->dirLight.enabled = false;
        app->pointLight.enabled = false;
        app->spotLight.enabled = false;
        if (key == GLFW_KEY_1) app->ambientLight.enabled = true;
        if (key == GLFW_KEY_2) app->dirLight.enabled = true;
        if (key == GLFW_KEY_3) app->pointLight.enabled = true;
        if (key == GLFW_KEY_4) app->spotLight.enabled = true;
    }
    // ESC fecha a janela
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void InputManager::setKeyCallback(std::function<void(int, int, int, int)> callback) {}
void InputManager::setMouseButtonCallback(std::function<void(int, int, int)> callback) {}
void InputManager::setMouseMoveCallback(std::function<void(double, double)> callback) {}
void InputManager::setScrollCallback(std::function<void(double, double)> callback) {}