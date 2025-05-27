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

void Renderer::renderScene(const Camera& camera, Mesh& tableMesh, std::vector<std::shared_ptr<ObjModelLoader>>& poolBalls, std::vector<glm::vec3>& ballPositions, const Window& window, const Light& light) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window.getHandle(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Renderizar mesa
    mainShader.use();
    glm::mat4 view = camera.getViewMatrix();
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

    // Renderizar todas as bolas
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Renderizar bola branca
    if (poolBalls.size() > 0) {
        ballShader.use();
        ballShader.setMat4("view", view);
        ballShader.setMat4("projection", proj);
        ballShader.setUniform("lightPos", light.getPosition());
        ballShader.setUniform("lightColor", light.getColor());
        ballShader.setUniform("viewPos", glm::vec3(view[3]));
        glUniform1i(glGetUniformLocation(ballShader.getProgramId(), "useTexture"), 0);
        glm::mat4 modelWhite = glm::mat4(1.0f);
        modelWhite = glm::translate(modelWhite, ballPositions[0]);
        modelWhite = glm::scale(modelWhite, glm::vec3(0.07f));
        poolBalls[0]->Render(ballPositions[0], glm::vec3(0,0,0), ballShader.getProgramId(), modelWhite, view, proj);
    }
    // Renderizar bolas coloridas
    for (size_t i = 1; i < poolBalls.size(); ++i) {
        ballShader.use();
        ballShader.setMat4("view", view);
        ballShader.setMat4("projection", proj);
        ballShader.setUniform("lightPos", light.getPosition());
        ballShader.setUniform("lightColor", light.getColor());
        ballShader.setUniform("viewPos", glm::vec3(view[3]));
        glUniform1i(glGetUniformLocation(ballShader.getProgramId(), "useTexture"), 1);
        glm::mat4 modelBall = glm::mat4(1.0f);
        modelBall = glm::translate(modelBall, ballPositions[i]);
        modelBall = glm::scale(modelBall, glm::vec3(0.07f));
        poolBalls[i]->Render(ballPositions[i], glm::vec3(0,0,0), ballShader.getProgramId(), modelBall, view, proj);
    }
}

void Renderer::renderMinimap(const Camera& camera, Mesh& tableMesh, std::vector<std::shared_ptr<ObjModelLoader>>& poolBalls, std::vector<glm::vec3>& ballPositions, const Window& window, const Light& light) {
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

    float minimapAspect = minimapWidth / minimapHeight;
    float tableHalfWidth = 1.5f;
    float tableHalfHeight = 0.75f;
    float orthoX = tableHalfWidth;
    float orthoY = tableHalfHeight;
    if (minimapAspect > (tableHalfWidth / tableHalfHeight)) {
        orthoX = minimapAspect * tableHalfHeight;
    } else {
        orthoY = tableHalfWidth / minimapAspect;
    }
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

    // Renderizar todas as bolas
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Renderizar bola branca
    if (poolBalls.size() > 0) {
        ballShader.use();
        ballShader.setMat4("view", view);
        ballShader.setMat4("projection", proj);
        ballShader.setUniform("lightPos", light.getPosition());
        ballShader.setUniform("lightColor", light.getColor());
        ballShader.setUniform("viewPos", glm::vec3(view[3]));
        glUniform1i(glGetUniformLocation(ballShader.getProgramId(), "useTexture"), 0);
        glm::mat4 modelWhite = glm::mat4(1.0f);
        modelWhite = glm::translate(modelWhite, ballPositions[0]);
        modelWhite = glm::scale(modelWhite, glm::vec3(0.07f));
        poolBalls[0]->Render(ballPositions[0], glm::vec3(0,0,0), ballShader.getProgramId(), modelWhite, view, proj);
    }
    // Renderizar bolas coloridas
    for (size_t i = 1; i < poolBalls.size(); ++i) {
        ballShader.use();
        ballShader.setMat4("view", view);
        ballShader.setMat4("projection", proj);
        ballShader.setUniform("lightPos", light.getPosition());
        ballShader.setUniform("lightColor", light.getColor());
        ballShader.setUniform("viewPos", glm::vec3(view[3]));
        glUniform1i(glGetUniformLocation(ballShader.getProgramId(), "useTexture"), 1);
        glm::mat4 modelBall = glm::mat4(1.0f);
        modelBall = glm::translate(modelBall, ballPositions[i]);
        modelBall = glm::scale(modelBall, glm::vec3(0.07f));
        poolBalls[i]->Render(ballPositions[i], glm::vec3(0,0,0), ballShader.getProgramId(), modelBall, view, proj);
    }
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