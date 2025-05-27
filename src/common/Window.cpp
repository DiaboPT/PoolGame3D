#include "Window.h"
#include <iostream>

using namespace PoolGame3D;

Window::Window() : window(nullptr), width(800), height(600) {}
Window::~Window() {
    if (window) glfwDestroyWindow(window);
}

bool Window::initialize(int w, int h, const std::string& title) {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return false;
    }
    // Configurar hints do GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (!window) return false;
    width = w;
    height = h;
    return true;
}

void Window::terminate() {
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
} 