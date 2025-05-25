#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "ObjModelLoader.h"

namespace PoolGame3D {

class App {
public:
    App();
    ~App();

    bool initialize();
    void run();

private:
    void update(float deltaTime);
    void render();

    bool isRunning;
    float lastFrameTime;
    Window window;
    InputManager inputManager;
    Camera camera;
    Mesh tableMesh;
    Mesh ballMesh;
    Renderer renderer;
};

} // namespace PoolGame3D