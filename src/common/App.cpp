#include "App.h"
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

App::App() : isRunning(false), lastFrameTime(0.0f) {}
App::~App() {}

bool App::initialize() {
    std::cout << "Iniciando aplicação..." << std::endl;
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return false;
    }
    std::cout << "GLFW inicializado com sucesso!" << std::endl;

    // Configurar hints do GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    if (!window.initialize(800, 600, "Pool Game 3D")) {
        std::cerr << "Falha ao criar janela GLFW!" << std::endl;
        glfwTerminate();
        return false;
    }
    std::cout << "Janela criada com sucesso!" << std::endl;

    glfwMakeContextCurrent(window.getHandle());
    std::cout << "Contexto OpenGL criado!" << std::endl;

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Falha ao inicializar GLEW: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(window.getHandle());
        glfwTerminate();
        return false;
    }
    std::cout << "GLEW inicializado com sucesso!" << std::endl;
    std::cout << "Versão do OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor do OpenGL: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer do OpenGL: " << glGetString(GL_RENDERER) << std::endl;

    // Verificar funções OpenGL
    if (!glfwGetProcAddress("glGenVertexArrays") ||
        !glfwGetProcAddress("glBindVertexArray") ||
        !glfwGetProcAddress("glGenBuffers")) {
        std::cerr << "Funções OpenGL não disponíveis!" << std::endl;
        return false;
    }

    if (!LoadOpenGLLibrary()) {
        std::cerr << "Failed to load OpenGL library!" << std::endl;
        glfwDestroyWindow(window.getHandle());
        glfwTerminate();
        return false;
    }
    std::cout << "OpenGL OK." << std::endl;

    // Inicializar input manager
    inputManager.initialize(window.getHandle());
    inputManager.setCamera(&camera);

    // Inicializar meshes
    if (!tableMesh.createParallelepiped()) {
        std::cerr << "Erro ao criar mesh da mesa!" << std::endl;
        return false;
    }
    if (!ballMesh.createSphere()) {
        std::cerr << "Erro ao criar mesh da bola!" << std::endl;
        return false;
    }

    // Inicializar renderer com shaders
    if (!renderer.initialize()) {
        std::cerr << "Erro ao inicializar renderer!" << std::endl;
        return false;
    }

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
    renderer.render(camera, tableMesh, ballMesh, window);
    
    // Renderiza o minimapa
    renderer.renderMinimap(camera, tableMesh, ballMesh);
} 