#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "ObjModelLoader.h"
#include "Light.h"
#include <vector>
#include <memory>

namespace PoolGame3D {

class App {
public:
    App();
    ~App();

    bool initialize();
    void run();

    // Luzes públicas para acesso do InputManager
    AmbientLight ambientLight;
    DirectionalLight dirLight;
    PointLight pointLight;
    SpotLight spotLight;

private:
    void update(float deltaTime);
    void render();

    bool initializeAllMeshes();

    bool isRunning;
    float lastFrameTime;
    Window window;
    InputManager inputManager;
    Camera camera;
    Mesh tableMesh;
    Mesh ballsMesh;
    Renderer renderer;
    Light sceneLight;
    std::vector<std::shared_ptr<ObjModelLoader>> poolBalls;
    std::vector<glm::vec3> ballPositions;
};

} // namespace PoolGame3D