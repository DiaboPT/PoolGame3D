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

#include "Header.h"

// Set up window title
const char* TITLE = "3D Pool Game";
// Set up window size
const int WIDTH = 800;
const int HEIGHT = 600;

// Variáveis globais para OpenGL
GLuint shaderProgram = 0;
GLuint VAO = 0, VBO = 0;

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

// Function to handle key events
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Se tecla 'Escape' premida
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

// Function to create the parallelepiped mesh
void CreateParallelepipedMesh(GLuint& VAO, GLuint& VBO) {
    // Cada face com cor diferente: posição (vec3) + cor (vec3)
    GLfloat vertices[] = {
        // +X
        +0.5f, -0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +0.0f, +0.0f,
        +0.5f, +0.5f, -0.5f, +1.0f, +0.0f, +0.0f,

        // -X
        -0.5f, -0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +0.0f, +1.0f, +0.0f,
        -0.5f, +0.5f, +0.5f, +0.0f, +1.0f, +0.0f,

        // +Y Verde
        -0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,
        -0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, +0.5f, +0.0f, +0.3f, +0.0f,
        +0.5f, +0.5f, -0.5f, +0.0f, +0.3f, +0.0f,

        // -Y
        -0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,
        -0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, -0.5f, +1.0f, +1.0f, +0.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +1.0f, +0.0f,

        // +Z
        -0.5f, -0.5f, +0.5f, +1.0f, +0.0f, +1.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +0.0f, +1.0f,
        -0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +1.0f,
        -0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +1.0f,
        +0.5f, -0.5f, +0.5f, +1.0f, +0.0f, +1.0f,
        +0.5f, +0.5f, +0.5f, +1.0f, +0.0f, +1.0f,

        // -Z
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

class PoolGame3DFrameWork : public FrameWork {

public:

    ~PoolGame3DFrameWork() {
        // Cleanup OpenGL resources
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (shaderProgram) glDeleteProgram(shaderProgram);
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
    void Start() override {

        // Checks if OpenGL is initialized
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return;
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
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetCursorPos(window, WIDTH * 0.5f, HEIGHT * 0.5f);

        // Checks if OpenGL Library is loaded
        if (!LoadOpenGLLibrary()) {
            std::cerr << "Failed to load OpenGL library!" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!" << std::endl;
            exit(-1);
        }
        // Create a VAO and VBO for the parallelepiped
        CreateParallelepipedMesh(VAO, VBO);

        if (VAO == 0) {
            std::cerr << "Erro ao criar o VAO!" << std::endl;
            exit(-1);
        }
        shaderProgram = CreateShaderProgram();

        if (shaderProgram == 0) {
            std::cerr << "Erro ao criar o shader program!" << std::endl;
            exit(-1);
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        glfwSetKeyCallback(window, keyCallback);

        // Set up the viewport and projection matrix
        glViewport(0, 0, WIDTH, HEIGHT);
        // Set up the projection matrix

        // glMatrixMode(GL_PROJECTION);
        // Set the projection matrix to identity

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }
    void InputManager() override {
        // Handle input events here
    }
    void Update(float deltaTime) override {
        // Update game state here
    }
};

// main function
int main() {

    PoolGame3DFrameWork* frameWork = new PoolGame3DFrameWork();
    frameWork->RunGame();
	delete frameWork;
    return 0;
}
