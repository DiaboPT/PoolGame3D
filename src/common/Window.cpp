#include "Window.h"
#include <iostream>

using namespace PoolGame3D;

Window::Window() : window(nullptr), width(800), height(600) {}
Window::~Window() {
    if (window) glfwDestroyWindow(window);
}

bool Window::initialize(int w, int h, const std::string& title) {
    window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (!window) return false;
    width = w;
    height = h;
    return true;
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