#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include "ShadersSources.h"

namespace PoolGame3D {

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);

    void use() const;
    void setUniform(const std::string& name, const glm::mat4& value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint programId;
    bool compileShader(GLuint& shaderId, GLenum type, const std::string& source);
    bool linkProgram();
};

} // namespace PoolGame3D