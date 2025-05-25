#include "Shader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace PoolGame3D;

Shader::Shader() : programId(0) {}

Shader::~Shader() {
    if (programId != 0) {
        glDeleteProgram(programId);
    }
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader;
    GLuint fragmentShader;
    
    if (!compileShader(vertexShader, GL_VERTEX_SHADER, vertexSource) ||
        !compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource)) {
        return false;
    }
    
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    
    if (!linkProgram()) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

void Shader::use() const {
    glUseProgram(programId);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLuint loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::compileShader(GLuint& shaderId, GLenum type, const std::string& source) {
    shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);
    
    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::linkProgram() {
    glLinkProgram(programId);
    
    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << "Shader link error: " << infoLog << std::endl;
        return false;
    }
    return true;
}