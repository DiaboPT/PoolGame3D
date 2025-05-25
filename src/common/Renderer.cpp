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
    setupMainViewport();
    
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
    float aspect = width / (float)height;
    glm::mat4 proj = camera.getProjectionMatrix(aspect);
    glm::mat4 mvp = proj * view * model;
    
    mainShader.setMat4("MVP", mvp);
    
    // Renderiza a mesa
    tableMesh.render();
    
    // Renderiza a bola
    ballMesh.render();
}

void Renderer::renderMinimap(const Camera& camera, const Mesh& tableMesh, const Mesh& ballMesh) {
    setupMinimapViewport();
    
    // Exemplo de renderização do minimapa (viewport reduzido)
    int miniW = 200, miniH = 150, miniX = 600, miniY = 450;
    glViewport(miniX, miniY, miniW, miniH);
    glEnable(GL_SCISSOR_TEST);
    glScissor(miniX, miniY, miniW, miniH);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    minimapShader.use();
    
    // Matrizes para top-down
    glm::mat4 topView = glm::lookAt(glm::vec3(0,5,0), glm::vec3(0,0,0), glm::vec3(0,0,-1));
    glm::mat4 topProj = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,0.1f,100.0f);
    glm::mat4 mvp = topProj * topView * glm::mat4(1.0f);
    
    minimapShader.setMat4("MVP", mvp);
    
    tableMesh.render();
    ballMesh.render();
    
    glDisable(GL_SCISSOR_TEST);
}

void Renderer::setupMainViewport() {
    glViewport(0, 0, 800, 600);
}

void Renderer::setupMinimapViewport() {
    glViewport(600, 450, 200, 150);
} 