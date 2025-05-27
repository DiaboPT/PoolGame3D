#include "App.h"
#include "OpenGLLoader.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SystemImports.h"
#include "ShadersSources.h"
#include <chrono>
#include <thread>

using namespace PoolGame3D;

App::App() : isRunning(false), lastFrameTime(0.0f), sceneLight(glm::vec3(2.0f, 4.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f) {}
App::~App() {}

bool App::initializeAllMeshes() {
    // Inicializar mesh da mesa
    if (!tableMesh.createParallelepiped()) {
        std::cerr << "Erro ao criar mesh da mesa!" << std::endl;
        return false;
    }

    // Criar posições das bolas (triângulo de sinuca alinhado ao eixo Z)
    float y = 0.20f;
    float ballDist = 0.14f;
    std::vector<glm::vec3> ballPositions;
    // Bola branca
    ballPositions.push_back(glm::vec3(0.0f, y, -1.05f));
    // Triângulo de 15 bolas
    int numRows = 5;
    float startZ = 0.55f;
    for (int row = 0; row < numRows; ++row) {
        int ballsInRow = row + 1;
        float z = startZ + row * ballDist;
        float xStart = -ballDist * row / 2.0f;
        for (int i = 0; i < ballsInRow; ++i) {
            float x = xStart + i * ballDist;
            ballPositions.push_back(glm::vec3(x, y, z));
        }
    }

    if (!ballsMesh.createMultipleSpheres(ballPositions)) {
        std::cerr << "Erro ao criar meshes das bolas!" << std::endl;
        return false;
    }

    return true;
}

bool App::initialize() {
    std::cout << "Iniciando aplicação..." << std::endl;
    if (!window.initialize(1200, 1000, "Pool Game 3D")) {
        std::cerr << "Falha ao criar janela GLFW!" << std::endl;
        return false;
    }
    std::cout << "Janela criada com sucesso!" << std::endl;

    if (!InitializeOpenGL(window)) {
        window.terminate();
        return false;
    }

    // Inicializar input manager
    inputManager.initialize(window.getHandle());
    inputManager.setCamera(&camera);

    // Inicializar todas as meshes (mesa e bolas)
    if (!initializeAllMeshes()) {
        return false;
    }

    // Inicializar renderer com shaders
    if (!renderer.initialize()) {
        std::cerr << "Erro ao inicializar renderer!" << std::endl;
        return false;
    }
    
    ConfigureOpenGL(window);

    isRunning = true;
    lastFrameTime = glfwGetTime();
    return true;
}

void App::run() {
    while (!window.shouldClose() && isRunning) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        update(deltaTime);
        render();

        window.swapBuffers();
        window.pollEvents();
    }
    glfwTerminate();
}

void App::update(float deltaTime) {
    // Atualiza a câmera
    camera.update();
}

void App::render() {
    // Renderiza a cena principal
    renderer.renderScene(camera, tableMesh, ballsMesh, window, sceneLight);
    
    // Renderiza o minimapa
    renderer.renderMinimap(camera, tableMesh, ballsMesh, window, sceneLight);
} 