#include "Header.h"

// Function declarations
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
void processInput(GLFWwindow* window);

const int verticesN = 3 * 6 * 6;

float verticesPositions[verticesN] = {

	// +X Direita
    +0.5f, -0.5f, +0.5f,
    +0.5f, -0.5f, -0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, -0.5f, -0.5f,
    +0.5f, +0.5f, -0.5f,

    // -X Esquerda
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, +0.5f,
    -0.5f, +0.5f, -0.5f,
    -0.5f, +0.5f, -0.5f,
    -0.5f, -0.5f, +0.5f,
    -0.5f, +0.5f, +0.5f,

    // +Y Cima Verde
    -0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, +0.5f,
    -0.5f, +0.5f, -0.5f,
    -0.5f, +0.5f, -0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, -0.5f,

    // -Y Baixo
    -0.5f, -0.5f, -0.5f,
    +0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, +0.5f,
    -0.5f, -0.5f, +0.5f,
    +0.5f, -0.5f, -0.5f,
    +0.5f, -0.5f, +0.5f,

    // +Z Frente
    -0.5f, -0.5f, +0.5f,
    +0.5f, -0.5f, +0.5f,
    -0.5f, +0.5f, +0.5f,
    -0.5f, +0.5f, +0.5f,
    +0.5f, -0.5f, +0.5f,
    +0.5f, +0.5f, +0.5f,

    // -Z Trás
    +0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    +0.5f, +0.5f, -0.5f,
    +0.5f, +0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, +0.5f, -0.5f,
};

float verticesNormals[verticesN] = {

    // +X Direita
    +1.0f, +0.0f, +0.0f,
    +1.0f, +0.0f, +0.0f,
    +1.0f, +0.0f, +0.0f,
    +1.0f, +0.0f, +0.0f,
    +1.0f, +0.0f, +0.0f,
    +1.0f, +0.0f, +0.0f,

    // -X Esquerda
    +0.0f, +1.0f, +0.0f,
    +0.0f, +1.0f, +0.0f,
    +0.0f, +1.0f, +0.0f,
    +0.0f, +1.0f, +0.0f,
    +0.0f, +1.0f, +0.0f,
    +0.0f, +1.0f, +0.0f,

    // +Y Cima Verde
    +0.0f, +0.3f, +0.0f,
    +0.0f, +0.3f, +0.0f,
    +0.0f, +0.3f, +0.0f,
    +0.0f, +0.3f, +0.0f,
    +0.0f, +0.3f, +0.0f,
    +0.0f, +0.3f, +0.0f,

    // -Y Baixo
    +1.0f, +1.0f, +0.0f,
    +1.0f, +1.0f, +0.0f,
    +1.0f, +1.0f, +0.0f,
    +1.0f, +1.0f, +0.0f,
    +1.0f, +1.0f, +0.0f,
    +1.0f, +1.0f, +0.0f,

    // +Z Frente
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,

    // -Z Trás
    +0.0f, +1.0f, +1.0f,
    +0.0f, +1.0f, +1.0f,
    +0.0f, +1.0f, +1.0f,
    +0.0f, +1.0f, +1.0f,
    +0.0f, +1.0f, +1.0f,
    +0.0f, +1.0f, +1.0f
};

float vertices[36 * 8]; // 8 floats per vertex: 3 pos + 3 normal + 2 uv

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
    std::vector<ObjModelLoader> balls;
    ObjModelLoader model;
    for (int i = 0; i <= 14; i++) {
        if (!model.Load("PoolBalls/Ball" + std::to_string(i +1) + ".obj")) {
            return -1;
        }
        model.NormalizeToUnit();
        model.Install();
        balls.push_back(model);
    }

    // Matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 5, 10), // Camera position
        glm::vec3(0, 0, 0), // Look at
        glm::vec3(0, 1, 0)  // Up
    );

    for (int i = 0; i < 36; ++i) {
        vertices[i * 8 + 0] = verticesPositions[i * 3 + 0];
        vertices[i * 8 + 1] = verticesPositions[i * 3 + 1];
        vertices[i * 8 + 2] = verticesPositions[i * 3 + 2];

        vertices[i * 8 + 3] = verticesNormals[i * 3 + 0];
        vertices[i * 8 + 4] = verticesNormals[i * 3 + 1];
        vertices[i * 8 + 5] = verticesNormals[i * 3 + 2];
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);

        // Compute matrices
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * modelMatrix;

        // Set uniforms
        GLuint mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        GLuint texLoc = glGetUniformLocation(shaderProgram, "u_Texture");
        glUniform1i(texLoc, 0); // Texture unit 0

        glActiveTexture(GL_TEXTURE0);

		// Draw the table
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Draw the balls
        for (int i = 0; i <= 15; i++)
            balls[i].Render(glm::vec3(i *2000, i *2000, i *2000), glm::vec3(0, 0, 0));

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
