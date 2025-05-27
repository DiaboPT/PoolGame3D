#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "App.h"
#include "OpenGLLoader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SystemImports.h"
#include "ShadersSources.h"
#include <chrono>
#include <thread>
#include "ObjModelLoader.h"

using namespace PoolGame3D;

App::App() : isRunning(false), lastFrameTime(0.0f), sceneLight(glm::vec3(2.0f, 4.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f) {
    // Inicializar luz ambiente
    ambientLight.color = glm::vec3(0.2f, 0.2f, 0.2f);
    ambientLight.intensity = 0.7f;
    ambientLight.enabled = true;

    // Inicializar luz direcional
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.color = glm::vec3(0.4f, 0.4f, 0.4f);
    dirLight.intensity = 1.2f;
    dirLight.enabled = true;

    // Inicializar luz pontual
    pointLight.position = glm::vec3(2.0f, 4.0f, 2.0f);
    pointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight.intensity = 1.0f;
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
    pointLight.enabled = true;

    // Inicializar luz cônica
    spotLight.position = glm::vec3(0.0f, 4.0f, 0.0f);
    spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.intensity = 2.5f;
    spotLight.cutOff = glm::cos(glm::radians(25.0f));
    spotLight.outerCutOff = glm::cos(glm::radians(30.0f));
    spotLight.constant = 1.0f;
    spotLight.linear = 0.09f;
    spotLight.quadratic = 0.032f;
    spotLight.enabled = true;
}

App::~App() {}

bool App::initializeAllMeshes() {
    // Inicializar mesh da mesa
    if (!tableMesh.createParallelepiped()) {
        std::cerr << "Erro ao criar mesh da mesa!" << std::endl;
        return false;
    }

    // Criar posições das bolas (triângulo de sinuca alinhado ao eixo Z)
    float y = 0.25f;
    float ballDist = 0.145f;
    ballPositions.clear();
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
    // Carregar bola branca
    poolBalls.clear();
    auto whiteBall = std::make_shared<ObjModelLoader>();
    if (whiteBall->Load("../src/resource/Sphere.obj", true)) {
        whiteBall->Install();
        poolBalls.push_back(whiteBall);
    }
    // Carregar bolas coloridas (1 a 15)
    for (int i = 1; i <= 15; ++i) {
        auto ball = std::make_shared<ObjModelLoader>();
        if (!ball->Load("../src/resource/Sphere.obj", true)) {
            continue;
        }
        std::string mtlPath = "../src/resource/PoolBalls/Ball" + std::to_string(i) + ".mtl";
        std::string texPath = "../src/resource/PoolBalls/PoolBalluv" + std::to_string(i) + ".jpg";
        ball->setMaterialAndTexture(mtlPath, texPath);
        ball->Install();
        poolBalls.push_back(ball);
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
    inputManager.setApp(this);

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
    renderer.renderScene(camera, tableMesh, poolBalls, ballPositions, window, sceneLight,
                        ambientLight, dirLight, pointLight, spotLight);
    renderer.renderMinimap(camera, tableMesh, poolBalls, ballPositions, window, sceneLight,
                          ambientLight, dirLight, pointLight, spotLight);
} 