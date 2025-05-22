/* 3D Pool Game
* 
* Summary:
* 	This work aims to apply the main 3D programming concepts covered in class. Thus, students, organized into
* 	groups of 4, must develop a program in C++ that, through the use of the OpenGL, GLEW, GLM and GLFW
* 	libraries, performs the rendering of a 3D scene, applying the respective textures, lighting, and performs an
* 	animation in response to a keyboard event.This work will result in the delivery, via Moodle, of the files with the
* 	source code, as well as an oral presentation / defense of the work carried out.
* 
* 	Objective:
* 	This work aims to develop a program in C++ (respecting the Object-Oriented Programming paradigm) that renders
* 	a 3D scene, applying the respective textures and lighting, and performs an animation (movement of one of the
* 	balls) in response to a keyboard event.
* 	The requirements are grouped here into 4 steps, in order to guide students in solving the work. Please note,
* 	however, that you should only submit a single version of your program (the final version), which meets all
* 	the requirements.
* 
* - Step 1 :
* 	• Implement window management and user interface through the GLFW library;
* 	• Implement the manipulation of matrices and vectors through the GLM library;
* 	• ( 1
* 	) Implement the rendering of a parallelepiped (to simulate the pool table) through the OpenGL library
* 	(including GLEW);
* 	• ( 2 ) The coloring of the parallelepiped fragments must be done so that each face has a distinct color (note
* 	that it is not expected that the lighting effects will be implemented at this stage);
* 	• The application should allow the user to zoom using the mouse scroll wheel ;
* 	• The application should allow navigation (rotation) around the center of the parallelepiped, through mouse
* 	movements.
* 	• Implement a minimap of the pool table, to be displayed in the top right corner of the application window.
* 	This minimap should show a top view perspective of the table. The minimap should not be affected by
* 	lighting.
* 
* - Step 2 :
* 	• Students must develop a C++ library (respecting the Object-Oriented Programming paradigm) capable of:
* 	o Load vertex position data, normals, and texture coordinates from .obj files;
* 	o Reorganize vertex information according to face information;
* 	o Send vertex data to GPU memory (VAO and respective VBO);
* 	o Identify the name of the .mtl file in the .obj file;
* 	o Load material properties (.mtl file), including texture image ;
* 	o Load the texture image into an OpenGL texture unit;
* 	o Provide function for rendering the object.
* 	Although the .mtl format supports multiple materials, for this work assume that a .mtl file will only have
* 	a single material to apply to the entire object.
* 	• The application must use this C++ library to load the models, compose and render a 3D scene that
* 	presents 15 balls arranged on a snooker table.
* 	Figure 1: Example of composition with billiard table and balls.
* 	• The camera should remain still in a position that allows the entire table to be viewed, oriented towards the
* 	center of the table.
* 	• The library must have its own namespace in which a class (and its respective member functions) will be
* 	defined to manage the rendering process. The following member functions must be implemented :
* 	o Load(const std::string obj_model_filepath)
* 	▪ This function aims to load the data from the .obj file, whose path is passed as an
* 	argument, into the CPU memory.
* 	▪ You must also load the materials and textures associated with the model into the CPU
* 	memory.
* 	o Install (void)
* 	ENGINEERING IN DIGITAL GAME DEVELOPMENT
* 	3D PROGRAMMING – 2ND YEAR Academic Year 2024/2025
* 	Polytechnic Institute of Cávado and Ave
* 	Higher School of Technology
* 	▪ Generates the necessary VAO and VBO and sends the model data (vertices, texture
* 	coordinates and normals) to GPU memory.
* 	o Render(glm::vec3 position, glm::vec3 orientation)
* 	▪ Function to render the model in the position defined by position and orientation defined
* 	by orientation .
* 	o Functions must also be created that allow the association between the model data (previously
* 	loaded to the GPU through the Install() function ) and the attributes of a shader program.
* 	o In addition to these mandatory functions, others may be created if deemed necessary.
* 
* - Step 3 :
* 	• It should be possible to apply any combination of 4 light sources that fall on the object:
* 	o Ambient light;
* 	o Directional light;
* 	o Spot light;
* 	o Conical light.
* 	• The parameters of each of the light sources are at the discretion of each group;
* 	• The application should allow the user to activate/deactivate each light source, using a key:
* 	o '1' – Enable/disable ambient light source;
* 	o '2' – Enable/disable directional light source;
* 	o '3' – Enable/disable point light source;
* 	o '4' – Enable/disable cone light source.
* 
* - Step 4 :
* 	• Implement the movement animation (translation and rotation) of one of the billiard balls. This animation
* 	should be triggered as soon as the user presses the “Space” key. The animation should stop when the ball
* 	hits another billiard ball or the table boundaries.
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <functional>
#include "OpenGLLoader.h"
#include <vector> // Add this include directive to resolve "std::vector"  

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

// Disable warnings for deprecated OpenGL functions
extern "C" {
    // This is for NVIDIA Optimus
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    // This is for AMD PowerXpress
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
    // This is for Intel Graphics
    __declspec(dllexport) DWORD IntelGraphicsRequestHighPerformance = 0x00000001;
}

using namespace std;

// Variáveis globais para OpenGL
GLuint shaderProgram = 0;
GLuint VAO = 0, VBO = 0;
GLuint sphereVAO, sphereVBO, sphereEBO;

// Camera
float cameraDistance = 5.0f;
float cameraPitch = -30.0f;
float cameraYaw = 0.0f;
float fov = 45.0f;

bool isRotating = false;
double lastX = 0.0f, lastY = 0.0f;

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 MVP;
    void main() {
      gl_Position = MVP * vec4(aPos, 1.0);
      ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
      FragColor = vec4(ourColor, 1.0);
    }
)";

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

GLuint CreateShaderProgram() {
    GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader link error: " << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
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

// Function to handle key events
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Se tecla 'Escape' premida
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

// Framework function
static void FrameWork(
    GLFWwindow*& window,
    function<void()> Start,
    function<void()> InputManager,
    function<void(float deltaTime)> Update,
    function<void()> Rendering) {

    Start();

    // Initialize lastTime
    float lastTime = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Calculate delta time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Call the input manager, update, and rendering functions
        InputManager();
        Update(deltaTime);
        Rendering();
    }

    // Clean up and close the window
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Function to create the parallelepiped mesh
void CreateParallelepipedMesh(GLuint& VAO, GLuint& VBO) {
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
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posição (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Cor (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Function to create a sphere mesh
void CreateSphereMesh(GLuint& VAO, GLuint& VBO, GLuint& EBO, int sectorCount = 36, int stackCount = 18) {
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

    // OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

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

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!isRotating) return;

    float sensitivity = 0.1f;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cameraYaw += xoffset * sensitivity;
    cameraPitch += yoffset * sensitivity;

    // Clamp pitch to prevent flipping
    if (cameraPitch > 30.0f) cameraPitch = 30.0f;
    if (cameraPitch < 30.0f) cameraPitch = -30.0f;
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;

    if (fov < 20.0f)
        fov = 20.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// main function
int main() {

    // Initialize OpenGL context
    GLFWwindow* window = nullptr;

    // Set up window size
    const int WIDTH = 800;
    const int HEIGHT = 600;

    // Set up window title
    const char* TITLE = "3D Pool Game";

    // Set up OpenGL context
    OpenGL_Context context;

    // Start function
    auto Start = [&]() {

        // Checks if OpenGL is initialized
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return -1;
        }

        // Set up hints for OpenGL context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        // Creates a window:
        // - size (WIDTH x HEIGHT)
        // - title (TITLE)
        // - monitor (monitor default)
        // - windowed mode (window)
        window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
        
        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSetCursorPos(window, WIDTH * 0.5f, HEIGHT * 0.5f);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetScrollCallback(window, ScrollCallback);

        // Checks if OpenGL Library is loaded
        if (!LoadOpenGLLibrary()) {
            std::cerr << "Failed to load OpenGL library!" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        glewExperimental = GL_TRUE; 
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!" << std::endl;
            return -1;
        }

		// Create a VAO and VBO for the parallelepiped
        CreateParallelepipedMesh(VAO, VBO);

		// Create a sphere mesh
        CreateSphereMesh(sphereVAO, sphereVBO, sphereEBO);

        if (VAO == 0) {
            std::cerr << "Erro ao criar o VAO!" << std::endl;
            return -1;
        }
        shaderProgram = CreateShaderProgram();

        if (shaderProgram == 0) {
            std::cerr << "Erro ao criar o shader program!" << std::endl;
            return -1;
        }

        // Initialize gl functions
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up input modes (inputs events)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        glfwSetKeyCallback(window, keyCallback);

        // Set up the viewport and projection matrix
        glViewport(0, 0, WIDTH, HEIGHT);
        // Set up the projection matrix

        // glMatrixMode(GL_PROJECTION);
        // Set the projection matrix to identity
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        };

    // Input manager function
    auto InputManager = [&]() {

        // Handle input events here
        };

    // Update function
    auto Update = [&](float deltaTime) {

        // Updatable game logic here
        };

    // Rendering function
    auto Rendering = [&]() {

		// Check if the shader program and VAO are initialized
        if (shaderProgram == 0 || VAO == 0) {
            std::cerr << "Shader program ou VAO não inicializado!" << std::endl;
            return;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // --- Main Camera Render -----

        // Renderização com shader e MVP
        glUseProgram(shaderProgram);

		// Set up the projection and view matrices
        glm::mat4 proj = glm::perspective(glm::radians(fov), WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        // Moving the world
        //glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::vec3 direction;
        direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        direction.y = sin(glm::radians(cameraPitch));
        direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        glm::vec3 cameraTarget = glm::normalize(direction);

        glm::vec3 cameraPos = -cameraTarget * cameraDistance;
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);

        glm::mat4 model = glm::mat4(1.0f);

        // Scale da Mesa 
        model = glm::scale(model, glm::vec3(1.5f, 0.3f, 3.0f));
        glm::mat4 mvp = proj * view * model;

		// Set the MVP matrix in the shader
        GLuint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		// Render the parallelepiped
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		// Render the sphere
        glBindVertexArray(sphereVAO);
        glDrawArrays(GL_TRIANGLES, 1, 36);

        // --- Minimap Render ---
        // Calculate minimap viewport size & position (top-right corner)
        int miniW = WIDTH / 4;
        int miniH = HEIGHT / 4;
        int miniX = WIDTH - miniW - 10;
        int miniY = HEIGHT - miniH - 10;

        // Set viewport for minimap
        glViewport(miniX, miniY, miniW, miniH);
        glEnable(GL_SCISSOR_TEST);
        glScissor(miniX, miniY, miniW, miniH);

        // Only clear depth buffer here to keep main view color visible
        glClear(GL_DEPTH_BUFFER_BIT);

        // Setup static top-down camera for minimap
        glm::vec3 topCamPos = glm::vec3(0.0f, 5.0f, 0.0f);
        glm::vec3 topCamTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 topCamUp = glm::vec3(0.0f, 0.0f, -1.0f); // "up" in top-down view
        
        // Orthographic projection: adjust these bounds to control visible area size
        float minimapAspect = miniW / (float)miniH;
        float tableHalfWidth = 1.5f * 0.01f;
        float tableHalfLenght = 3.0f * 0.01f;

        if (minimapAspect > 1.0f)
            tableHalfLenght *= minimapAspect;
        else
            tableHalfWidth *= minimapAspect;

        float padding = 0.5f;
        tableHalfLenght += padding;
        tableHalfWidth += padding;

        glm::mat4 topProjection = glm::ortho(-tableHalfWidth, tableHalfWidth, // Left, Right
                                             -tableHalfLenght, tableHalfLenght, // Bottom, Top
                                             0.1f, 100.0f);        
        //glm::mat4 topProjection = glm::perspective(glm::radians(110.0f), miniW / (float)miniH, 0.1f, 100.0f);

        glm::mat4 topModel = glm::mat4(1.0f);
        glm::mat4 topView = glm::lookAt(topCamPos, topCamTarget, topCamUp);
        glm::mat4 topMVP = topProjection * topView * topModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(topMVP));

        // Draw the table again in minimap
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw balls again
        glBindVertexArray(sphereVAO);
        glDrawArrays(GL_TRIANGLES, 1, 36);

        glDisable(GL_SCISSOR_TEST);

        // Reset viewport to full screen for next frame
        glViewport(0, 0, WIDTH, HEIGHT);

		// Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        };

    // Call the framework function
    FrameWork(window, Start, InputManager, Update, Rendering);

    return 0;
}
