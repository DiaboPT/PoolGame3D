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
    void render() const;
    
private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

} // namespace PoolGame3D