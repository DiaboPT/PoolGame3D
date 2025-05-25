#include "Mesh.h"
#include <iostream>

using namespace PoolGame3D;

Mesh::Mesh() : VAO(0), VBO(0), EBO(0) {}

Mesh::~Mesh() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

bool Mesh::createParallelepiped() {
    std::cout << "Iniciando criação do mesh..." << std::endl;
    
    // Cada face com normal diferente: posição (vec3) + normal (vec3)
    GLfloat vertices[] = {
        // +X Direita
        +0.5f, -0.5f, +0.5f,  1.0f, 0.0f, 0.0f,
        +0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        +0.5f, +0.5f, +0.5f,  1.0f, 0.0f, 0.0f,
        +0.5f, +0.5f, +0.5f,  1.0f, 0.0f, 0.0f,
        +0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        +0.5f, +0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

        // -X Esquerda
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, +0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, +0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, +0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, +0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, +0.5f, +0.5f,  -1.0f, 0.0f, 0.0f,

        // +Y Cima
        -0.5f, +0.5f, +0.5f,  0.0f, 1.0f, 0.0f,
        +0.5f, +0.5f, +0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, +0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, +0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        +0.5f, +0.5f, +0.5f,  0.0f, 1.0f, 0.0f,
        +0.5f, +0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        // -Y Baixo
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
        +0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, +0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, +0.5f,  0.0f, -1.0f, 0.0f,
        +0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
        +0.5f, -0.5f, +0.5f,  0.0f, -1.0f, 0.0f,

        // +Z Frente
        -0.5f, -0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        +0.5f, -0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, +0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, +0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        +0.5f, -0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        +0.5f, +0.5f, +0.5f,  0.0f, 0.0f, 1.0f,

        // -Z Trás
        +0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
        +0.5f, +0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
        +0.5f, +0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
        -0.5f, +0.5f, -0.5f,  0.0f, 0.0f, -1.0f
    };

    std::cout << "Gerando VAO..." << std::endl;
    GLuint tempVAO = 0;
    glGenVertexArrays(1, &tempVAO);
    if (tempVAO == 0) {
        std::cerr << "Erro ao gerar VAO!" << std::endl;
        return false;
    }
    VAO = tempVAO;
    std::cout << "VAO gerado: " << VAO << std::endl;

    std::cout << "Gerando VBO..." << std::endl;
    GLuint tempVBO = 0;
    glGenBuffers(1, &tempVBO);
    if (tempVBO == 0) {
        std::cerr << "Erro ao gerar VBO!" << std::endl;
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
        return false;
    }
    VBO = tempVBO;
    std::cout << "VBO gerado: " << VBO << std::endl;

    std::cout << "Configurando VAO..." << std::endl;
    glBindVertexArray(VAO);
    
    std::cout << "Configurando VBO..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::cout << "Configurando atributos..." << std::endl;
    // Posição (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    std::cout << "Limpando bindings..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    std::cout << "Mesh criado com sucesso!" << std::endl;
    return true;
}

bool Mesh::createSphere(int sectorCount, int stackCount) {
     std::cout << "Iniciando criação da esfera..." << std::endl;
    
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    float x, y, z, xy;                          // position
    float nx, ny, nz, lengthInv = 1.0f;         // normal (optional)
    float s, t;                                 // texture coord (optional)
    float radius = 0.5f;

    const float PI = 3.14159265359f;
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * stackStep;        // from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * sectorStep;

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // color (use normalized position as color)
            vertices.push_back((x + 0.5f));  // R
            vertices.push_back((y + 0.5f));  // G
            vertices.push_back((z + 0.5f));  // B
        }
    }

    // Indices
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    std::cout << "Gerando VAO da esfera..." << std::endl;
    GLuint tempVAO = 0;
    glGenVertexArrays(1, &tempVAO);
    if (tempVAO == 0) {
        std::cerr << "Erro ao gerar VAO da esfera!" << std::endl;
        return false;
    }
    VAO = tempVAO;
    std::cout << "VAO da esfera gerado: " << VAO << std::endl;

    std::cout << "Gerando VBO da esfera..." << std::endl;
    GLuint tempVBO = 0;
    glGenBuffers(1, &tempVBO);
    if (tempVBO == 0) {
        std::cerr << "Erro ao gerar VBO da esfera!" << std::endl;
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
        return false;
    }
    VBO = tempVBO;
    std::cout << "VBO da esfera gerado: " << VBO << std::endl;

    std::cout << "Gerando EBO da esfera..." << std::endl;
    GLuint tempEBO = 0;
    glGenBuffers(1, &tempEBO);
    if (tempEBO == 0) {
        std::cerr << "Erro ao gerar EBO da esfera!" << std::endl;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        VAO = 0;
        VBO = 0;
        return false;
    }
    EBO = tempEBO;
    std::cout << "EBO da esfera gerado: " << EBO << std::endl;

    std::cout << "Configurando VAO da esfera..." << std::endl;
    glBindVertexArray(VAO);

    std::cout << "Configurando VBO da esfera..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    std::cout << "Configurando EBO da esfera..." << std::endl;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    std::cout << "Configurando atributos da esfera..." << std::endl;
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    std::cout << "Limpando bindings da esfera..." << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    std::cout << "Esfera criada com sucesso!" << std::endl;
    return true;
}

void Mesh::render() const {
    glBindVertexArray(VAO);
    if (EBO != 0) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        // Para o paralelepípedo, temos 36 vértices (6 faces * 2 triângulos * 3 vértices)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}