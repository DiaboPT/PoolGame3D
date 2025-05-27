#include "Header.h"

// Function declarations
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
void processInput(GLFWwindow* window);

int main() {
    // GLFW + GLEW init
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "OBJ Loader Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return -1;

    // Load shader
    GLuint shaderProgram = LoadShaders("shader.vert", "shader.frag");

    // Load model
    ObjModelLoader model;
    if (!model.Load("PoolBalls/Ball1.obj")) {
        return -1;
    }
    model.Install();

    // Matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 2, 5), // Camera position
        glm::vec3(0, 0, 0), // Look at
        glm::vec3(0, 1, 0)  // Up
    );

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));

        glm::mat4 mvp = projection * view * modelMatrix;

        GLuint mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        GLuint texLoc = glGetUniformLocation(shaderProgram, "u_Texture");
        glUniform1i(texLoc, 0); // Texture unit 0

        glActiveTexture(GL_TEXTURE0);
        model.Render(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
    // Read Vertex Shader code
    std::ifstream vertexFile(vertex_file_path);
    std::stringstream vertexBuffer;
    vertexBuffer << vertexFile.rdbuf();
    std::string vertexCode = vertexBuffer.str();
    const char* vertexSource = vertexCode.c_str();

    // Read Fragment Shader code
    std::ifstream fragmentFile(fragment_file_path);
    std::stringstream fragmentBuffer;
    fragmentBuffer << fragmentFile.rdbuf();
    std::string fragmentCode = fragmentBuffer.str();
    const char* fragmentSource = fragmentCode.c_str();

    // Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Check Vertex Shader
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader compilation failed:\n" << infoLog << std::endl;
    }

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader compilation failed:\n" << infoLog << std::endl;
    }

    // Link Program
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // Check Linking
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader Program linking failed:\n" << infoLog << std::endl;
    }

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programID;
}

// Simple input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
