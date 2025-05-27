#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Light.h"
#include "ObjModelLoader.h"
#include <vector>
#include <memory>

namespace PoolGame3D {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void renderScene(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light);
    void renderMinimap(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light);
    void renderScene(const Camera& camera, Mesh& tableMesh, std::vector<std::shared_ptr<ObjModelLoader>>& poolBalls, 
                    std::vector<glm::vec3>& ballPositions, const Window& window, const Light& light,
                    const AmbientLight& ambientLight, const DirectionalLight& dirLight,
                    const PointLight& pointLight, const SpotLight& spotLight);
    void renderMinimap(const Camera& camera, Mesh& tableMesh, std::vector<std::shared_ptr<ObjModelLoader>>& poolBalls,
                      std::vector<glm::vec3>& ballPositions, const Window& window, const Light& light,
                      const AmbientLight& ambientLight, const DirectionalLight& dirLight,
                      const PointLight& pointLight, const SpotLight& spotLight);

private:
    PoolGame3D::Shader mainShader;
    PoolGame3D::Shader ballShader;
    
    void setupMainViewport();
    void setupMinimapViewport(const Window& window);
    void renderMeshes(const Camera& camera, Mesh& tableMesh, Mesh& ballsMesh, const Window& window, const Light& light,
                     const AmbientLight& ambientLight, const DirectionalLight& dirLight,
                     const PointLight& pointLight, const SpotLight& spotLight);
};

} // namespace PoolGame3D