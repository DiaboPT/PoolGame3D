#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace PoolGame3D {

class Window {
public:
    Window();
    ~Window();

    bool initialize(int width, int height, const std::string& title);
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    
    GLFWwindow* getHandle() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    GLFWwindow* window;
    int width = 800;
    int height = 600;
};

} // namespace PoolGame3D