#include "Renderer.h"
#include <iostream>
#include "ShadersSources.h"

using namespace PoolGame3D;

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::initialize() {
    // Inicialização dos shaders
    if (!mainShader.loadFromSource(vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Erro ao carregar shader principal!" << std::endl;
        return false;
    }
    
    if (!minimapShader.loadFromSource(vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Erro ao carregar shader do minimapa!" << std::endl;
        return false;
    }
    
    return true;
}

void Renderer::render(const Camera& camera, const Mesh& tableMesh, const Mesh& ballMesh, const Window& window) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.getHandle(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    
    // Limpa a tela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Usa o shader principal
    mainShader.use();
    
    // Define as matrizes
    glm::mat4 model = glm::mat4(1.0f);
    // Escala para mesa: largura, altura, comprimento
    model = glm::scale(model, glm::vec3(1.5f, 0.3f, 3.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 view = camera.getViewMatrix();
    
    int width, height;
    glfwGetFramebufferSize(window.getHandle(), &width, &height);
    float aspect = fbWidth / (float)fbHeight;
    glm::mat4 proj = camera.getProjectionMatrix(aspect);
    glm::mat4 mvp = proj * view * model;
    
    mainShader.setMat4("MVP", mvp);
    
    // Renderiza a mesa
    tableMesh.render();
    
    // Renderiza a bola
    ballMesh.render();
}

void Renderer::renderMinimap(const Camera& camera, const Mesh& tableMesh, const Mesh& ballMesh, const Window& window) {
    setupMinimapViewport(window);
    
    // Obtém as dimensões da janela
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window.getHandle(), &windowWidth, &windowHeight);
    
    float minimapWidth = windowWidth * 0.2f;
    float minimapHeight = minimapWidth * 1.0f;
    int minimapX = windowWidth - minimapWidth - 10;
    int minimapY = 10;
    glViewport(minimapX, minimapY, minimapWidth, minimapHeight);
    glScissor(minimapX, minimapY, minimapWidth, minimapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    minimapShader.use();
    
    // Ajusta a matriz de projeção para manter a proporção correta da mesa
    glm::mat4 topView = glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    glm::mat4 topProj = glm::ortho(-1.5f, 1.5f, -0.75f, 0.75f, 0.1f, 100.0f);
    glm::mat4 mvp = topProj * topView * glm::mat4(1.0f);
    
    minimapShader.setMat4("MVP", mvp);
    
    tableMesh.render();
    ballMesh.render();
    
    glDisable(GL_SCISSOR_TEST);
}

void Renderer::setupMainViewport() {
    glViewport(0, 0, 800, 600);
}

void Renderer::setupMinimapViewport(const Window& window) {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window.getHandle(), &windowWidth, &windowHeight);
    
    float minimapWidth = windowWidth * 0.2f;
    float minimapHeight = minimapWidth * 0.5f; // proporção 2:1 (3:1.5)
    int minimapX = windowWidth - minimapWidth - 10;
    int minimapY = 10;
    
    glViewport(minimapX, minimapY, minimapWidth, minimapHeight);
} 