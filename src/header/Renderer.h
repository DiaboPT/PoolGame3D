#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Light.h"

namespace PoolGame3D {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void renderScene(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light);
    void renderMinimap(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light);

private:
    PoolGame3D::Shader mainShader;
    PoolGame3D::Shader ballShader;
    
    void setupMainViewport();
    void setupMinimapViewport(const Window& window);
    void renderMeshes(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light);
};

} // namespace PoolGame3D