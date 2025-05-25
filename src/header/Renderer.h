#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

namespace PoolGame3D {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void render(const Camera& camera, const Mesh& tableMesh, const Mesh& ballMesh, const Window& window);
    void renderMinimap(const Camera& camera, const Mesh& tableMesh, const Mesh& ballMesh);

private:
    PoolGame3D::Shader mainShader;
    PoolGame3D::Shader minimapShader;
    
    void setupMainViewport();
    void setupMinimapViewport();
};

} // namespace PoolGame3D