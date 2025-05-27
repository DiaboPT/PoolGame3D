#pragma once
#include <functional>
#include "Camera.h"
#include <GLFW/glfw3.h>

namespace PoolGame3D {

// Forward declaration
class App;

class InputManager {
public:
    InputManager();
    ~InputManager();

    void initialize(GLFWwindow* window);
    
    // Callbacks
    void setKeyCallback(std::function<void(int, int, int, int)> callback);
    void setMouseButtonCallback(std::function<void(int, int, int)> callback);
    void setMouseMoveCallback(std::function<void(double, double)> callback);
    void setScrollCallback(std::function<void(double, double)> callback);

    void setCamera(Camera* cam) { camera = cam; }
    void setApp(App* appPtr) { app = appPtr; }

private:
    GLFWwindow* window;
    Camera* camera = nullptr;
    App* app = nullptr;
    bool isRotating = false;
    double lastMouseX = 0.0, lastMouseY = 0.0;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

} // namespace PoolGame3D