#pragma once
#include "Header.h"

// Variáveis globais para OpenGL
extern GLuint shaderProgram;
extern GLuint VAO, VBO;

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;

GLuint CompileShader(GLenum type, const char* src);

GLuint CreateShaderProgram();

// Function to set up an Object
struct OpenGL_Context {

    string* object_Window_Target = nullptr;
    // 6 faces, 2 triangles/face, 3 vertices/triangle 
    static constexpr GLint numVertices = 6 * 2 * 3;

    // numVertices * xyz/vertice
    GLfloat vertices[numVertices * 3] = {
        // +X (Face #0)
        +0.5f, -0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f,
        +0.5f, +0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f,
        // -X (Face #1)
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
        // +Y (Face #2)
        -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
        -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f,
        // -Y (Face #3)
        -0.5f, -0.5f, -0.5f, +0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f,
        -0.5f, -0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f,
        // +Z (Face #4)
        -0.5f, -0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
        -0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f,
        // -Z (Face #5)
        +0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f,
        +0.5f, +0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f
    };

    // Estrutura para armazenar o VAO e VBO do paralelepípedo
    struct TableMesh {
        GLuint VAOMesh, VBOMesh;
    };
    TableMesh table;

};

// Function to handle key events
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Function to create the parallelepiped mesh
void CreateParallelepipedMesh(GLuint& VAO, GLuint& VBO);
