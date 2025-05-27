#include "Renderer.h"
#include <iostream>
#include "ShadersSources.h"

using namespace PoolGame3D;

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::initialize() {
    if (!mainShader.loadFromSource(vertexShaderTable, fragmentShaderTable)) {
        std::cerr << "Erro ao carregar shader principal!" << std::endl;
        return false;
    }
    
    if (!ballShader.loadFromSource(vertexShaderBall, fragmentShaderBall)) {
        std::cerr << "Erro ao carregar shader das bolas!" << std::endl;
        return false;
    }
    
    return true;
}

void Renderer::renderMeshes(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light) {
    // Shader da mesa com luz
    mainShader.use();
    glm::mat4 view = camera.getViewMatrix();
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.getHandle(), &fbWidth, &fbHeight);
    float aspect = fbWidth / (float)fbHeight;
    glm::mat4 proj = camera.getProjectionMatrix(aspect);
    glm::mat4 modelTable = glm::mat4(1.0f);
    modelTable = glm::scale(modelTable, glm::vec3(1.5f, 0.3f, 3.0f));
    mainShader.setMat4("model", modelTable);
    mainShader.setMat4("view", view);
    mainShader.setMat4("projection", proj);
    mainShader.setUniform("lightPos", light.getPosition());
    mainShader.setUniform("lightColor", light.getColor());
    mainShader.setUniform("viewPos", glm::vec3(view[3]));
    tableMesh.render();

    // Shader das bolas com luz
    ballShader.use();
    ballShader.setMat4("view", view);
    ballShader.setMat4("projection", proj);
    ballShader.setUniform("lightPos", light.getPosition());
    ballShader.setUniform("lightColor", light.getColor());
    ballShader.setUniform("viewPos", glm::vec3(view[3]));
    ballsMesh.render();
}

void Renderer::renderScene(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.getHandle(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderMeshes(camera, tableMesh, ballsMesh, window, light);
}

void Renderer::renderMinimap(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light) {
    setupMinimapViewport(window);
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window.getHandle(), &windowWidth, &windowHeight);
    float minimapWidth = windowWidth * 0.2f;
    float minimapHeight = minimapWidth * 1.0f;
    int minimapX = windowWidth - minimapWidth - 10;
    int minimapY = 10;
    glViewport(minimapX, minimapY, minimapWidth, minimapHeight);
    glScissor(minimapX, minimapY, minimapWidth, minimapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Aspect ratio do minimapa
    float minimapAspect = minimapWidth / minimapHeight;
    float tableHalfWidth = 1.5f;  // metade do comprimento da mesa (x)
    float tableHalfHeight = 0.75f; // metade da largura da mesa (z)
    float orthoX = tableHalfWidth;
    float orthoY = tableHalfHeight;
    if (minimapAspect > (tableHalfWidth / tableHalfHeight)) {
        // Viewport mais larga que a mesa: aumenta X
        orthoX = minimapAspect * tableHalfHeight;
    } else {
        // Viewport mais alta que a mesa: aumenta Y
        orthoY = tableHalfWidth / minimapAspect;
    }

    // Matrizes de câmera top view (up corrigido)
    glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    glm::mat4 proj = glm::ortho(-orthoX, orthoX, -orthoY, orthoY, 0.1f, 100.0f);

    // Renderizar mesa
    mainShader.use();
    glm::mat4 modelTable = glm::mat4(1.0f);
    modelTable = glm::scale(modelTable, glm::vec3(1.5f, 0.3f, 3.0f));
    mainShader.setMat4("model", modelTable);
    mainShader.setMat4("view", view);
    mainShader.setMat4("projection", proj);
    mainShader.setUniform("lightPos", light.getPosition());
    mainShader.setUniform("lightColor", light.getColor());
    mainShader.setUniform("viewPos", glm::vec3(view[3]));
    tableMesh.render();

    // Renderizar bolas
    ballShader.use();
    ballShader.setMat4("view", view);
    ballShader.setMat4("projection", proj);
    ballShader.setUniform("lightPos", light.getPosition());
    ballShader.setUniform("lightColor", light.getColor());
    ballShader.setUniform("viewPos", glm::vec3(view[3]));
    ballsMesh.render();

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