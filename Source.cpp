#pragma region includes

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <vector> // Add this include directive to resolve "std::vector"  

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <windows.h>
#include <functional>

#include "OpenGLLoader.h"
#include "stb_image.h"
#include "ObjModelLoader.h"
#include "Camera.h"
#include "LoadShaders.h"
#include "Light.h"

#define STB_IMAGE_IMPLEMENTATION

// Disable warnings for deprecated OpenGL functions
extern "C" {
    // This is for NVIDIA Optimus
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    // This is for AMD PowerXpress
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
    // This is for Intel Graphics
    __declspec(dllexport) DWORD IntelGraphicsRequestHighPerformance = 0x00000001;
}
#pragma endregion

using namespace PoolGame3D;
using namespace std;

// Zoom level
float zoomLevel = 1.0f;
glm::mat4 modelMatrix = glm::mat4(1.0f);

// Variáveis globais para OpenGL
GLuint shaderProgram, sPositions, sNormals, sTexcoords, sTexture;

GLuint VAO = 0, VBO = 0;

// Ball vector
vector<ObjModelLoader> ballsArray;
vector<glm::vec3> ballPositions = {
    // Ball 1 (tip)
    glm::vec3(0.0f, 2.5f, -12.0f), // Ball 1

    // Row 2 (2 balls)
    glm::vec3(-1.1f, 2.5f, -1.1f), // Ball 2
    glm::vec3(1.1f, 2.5f, -1.1f),  // Ball 3

    // Row 3 (3 balls)
    glm::vec3(-2.2f, 2.5f, 0.8f),  // Ball 4
    glm::vec3(0.0f, 2.5f, 0.8f),   // Ball 5
    glm::vec3(2.2f, 2.5f, 0.8f),   // Ball 6

    // Row 4 (4 balls)
    glm::vec3(-3.3f, 2.5f, 2.7f),  // Ball 7
    glm::vec3(-1.1f, 2.5f, 2.7f),  // Ball 8
    glm::vec3(1.1f, 2.5f, 2.7f),   // Ball 9
    glm::vec3(3.3f, 2.5f, 2.7f),   // Ball 10

    // Row 5 (5 balls)
    glm::vec3(-4.4f, 2.5f, 4.6f),  // Ball 11
    glm::vec3(-2.2f, 2.5f, 4.6f),  // Ball 12
    glm::vec3(0.0f, 2.5f, 4.6f),   // Ball 13
    glm::vec3(2.2f, 2.5f, 4.6f),   // Ball 14
    glm::vec3(4.4f, 2.5f, 4.6f)    // Ball 15
};

vector<glm::vec3> ballOrientations = {
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
    glm::vec3(-1.0f, 2.5f, 5.0f),
};

// Camera
float cameraDistance = 5.0f;
float cameraPitch = -30.0f;
float cameraYaw = 0.0f;
float fov = 90.0f;

bool isRotating = false;
double lastX = 0.0f, lastY = 0.0f;

GLuint textureShaderProgram = 0;

GLuint CompileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

void CreateParallelepipedMesh() {
    // Cada face com cor diferente: posição (vec3) + cor (vec3)
    GLfloat vertices[] = {
        // +X Direita Vermelho
        +0.5f, -0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, -0.5f, +1.0f, +0.0f, +0.0f,

        // -X Esquerda
        -0.5f, -0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, +0.5f, +0.0f, +1.0f, +0.0f,

        // +Y Cima Verde
        -0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,

        // -Y Baixo
        -0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,

        // +Z Frente
        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,
        -0.5f, +0.5f, +0.5f, +1.0f, +1.0f, +1.0f,
        -0.5f, +0.5f, +0.5f, +1.0f, +1.0f, +1.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +1.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +1.0f, +1.0f,

        // -Z Trás
        +0.5f, -0.5f, -0.5f, +0.0f, +1.0f, +1.0f,
        -0.5f, -0.5f, -0.5f, +0.0f, +1.0f, +1.0f,
        +0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +1.0f,
        +0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +1.0f,
        -0.5f, -0.5f, -0.5f, +0.0f, +1.0f, +1.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +1.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    // Posição (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Cor (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


#pragma region Callbacks
/// <summary>
/// Method to handle when mouse button is pressed
/// </summary>
/// <param name="window"></param>
/// <param name="button"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isRotating = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE) {
            isRotating = false;
        }
    }
}

/// <summary>
/// Method to handle mouse position
/// </summary>
/// <param name="window"></param>
/// <param name="xpos"></param>
/// <param name="ypos"></param>
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {

    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    // Apply rotation to the model matrix
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians((float)deltaX), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians((float)deltaY), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = rotation * modelMatrix;

    // Update last mouse position
    lastX = xpos;
    lastY = ypos;
}

/// <summary>
/// Method to handle mouse scroll
/// </summary>
/// <param name="window"></param>
/// <param name="xoffset"></param>
/// <param name="yoffset"></param>
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    float zoomAmount = 0.1f;
    zoomLevel += yoffset * zoomAmount;
}

/// <summary>
/// Method to handle keyboard
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Se tecla 'Escape' premida
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}
#pragma endregion

void init() {
    glClearColor(0.02f, 0.0f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "light.vert" },
        { GL_FRAGMENT_SHADER, "light.frag" },
        { GL_NONE, NULL }
    };

    shaderProgram = LoadShaders(shaders);

    glUseProgram(shaderProgram);

    sPositions = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexPosition");
    sNormals = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vertexNormals");
    sTexcoords = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "texCoords");

    sTexture = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "texSampler");

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << error << std::endl;
    }

}

// main function
int main() {

    // Checks if OpenGL is initialized
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    cam::Camera* camera;
    camera = camera->GetInstance();

    // Initialize OpenGL context
    GLFWwindow* window = nullptr;

    // Set up window size
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const char* TITLE = "3D Pool Game";

    window = glfwCreateWindow(1600, 900, "Window", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Set up OpenGL context
    //OpenGL_Context context;

    glewInit();
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    init();

    // Callback
    glfwSetCursorPos(window, WIDTH * 0.5f, HEIGHT * 0.5f);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    CreateParallelepipedMesh();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Array with all obj files
    vector<string> objFiles{
        "PoolBalls/Ball1.obj",
        "PoolBalls/Ball2.obj",
        "PoolBalls/Ball3.obj",
        "PoolBalls/Ball4.obj",
        "PoolBalls/Ball5.obj",
        "PoolBalls/Ball6.obj",
        "PoolBalls/Ball7.obj",
        "PoolBalls/Ball8.obj",
        "PoolBalls/Ball9.obj",
        "PoolBalls/Ball10.obj",
        "PoolBalls/Ball11.obj",
        "PoolBalls/Ball12.obj",
        "PoolBalls/Ball13.obj",
        "PoolBalls/Ball14.obj",
        "PoolBalls/Ball15.obj",
    };

    int counter = 0;
    for (string i : objFiles) {
        ObjModelLoader obj;
        obj.Load(i, sPositions, sNormals, sTexcoords, sTexture, shaderProgram, counter++);
        obj.Install();
        //lighting::Lights(&obj, shaderProgram);
        std::cout << "ShaderProgram: " << shaderProgram << " at " << i << std::endl;

        ballsArray.push_back(obj);
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the projection and view matrices
        glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoomLevel));
        glm::mat4 mvp = camera->projection * camera->view * modelMatrix * zoomMatrix;

        glUseProgram(shaderProgram);

        int i = 0;
        for (auto& obj : ballsArray) {
            //if (i == 0 && ballIsMoving) {
            //
            //    // Position
            //    ballPositions[i].x += 0;
            //    ballPositions[i].z += moveSpeed;

            //    // Rotation
            //    ballOrientations[i].x += rotationSpeed;

            //    ballIsMoving = checkCollisions();
            //}
            obj.Render(ballPositions[i], ballOrientations[i], modelMatrix * zoomMatrix);
			i++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// Function to set up an Object
struct OpenGL_Context {
    // Initialize the member variable 
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
        GLuint VAO, VBO;
    };
    TableMesh table;

};

// Function to create the parallelepiped mesh
