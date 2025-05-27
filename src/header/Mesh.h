#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace PoolGame3D {

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    bool createParallelepiped();
    bool createSphere(int sectorCount = 36, int stackCount = 18);
    bool createMultipleSpheres(const std::vector<glm::vec3>& positions);
    void render() const;
    bool initializeAllMeshes();
    
private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> spherePositions;
    bool isMultipleSpheres = false;
};

} // namespace PoolGame3D