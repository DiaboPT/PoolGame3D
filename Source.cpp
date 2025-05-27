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
    -0.5f, -0.5f, -0.5f,
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
    // +X
    1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0, 1,0,0,
    // -X
    -1,0,0, -1,0,0, -1,0,0, -1,0,0, -1,0,0, -1,0,0,
    // +Y
    0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0, 0,1,0,
    // -Y
    0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
    // +Z
    0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1,
    // -Z
    0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1
};

float vertices[36 * 8]; // 8 floats per vertex: 3 pos + 3 normal

// Camera
float cameraDistance = 5.0f;
float cameraPitch = -30.0f;
float cameraYaw = 0.0f;
float fov = 45.0f;

bool isRotating = false;
double lastX = 0.0f, lastY = 0.0f;

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

int main() {

    // Initialize OpenGL context
    GLFWwindow* window = nullptr;

    // Set up window size
    const int WIDTH = 800;
    const int HEIGHT = 600;

    // Set up window title
    const char* TITLE = "3D Pool Game";

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Load shader
    GLuint shaderProgram = LoadShaders("glsl shader.vert", "glsl shader.frag");
    if (shaderProgram == 0) {
        std::cerr << "Erro ao criar o shader program!" << std::endl;
        return -1;
    }

    // Load model
    std::vector<ObjModelLoader> balls;
    for (int i = 0; i <= 14; i++) {
        ObjModelLoader model;
        if (!model.Load("PoolBalls/Ball" + std::to_string(i +1) + ".obj")) {
            return -1;
        }
        model.NormalizeToUnit();
        model.Install();
        balls.push_back(model);
    }

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);
        // Set up the projection and view matrices
        glm::mat4 proj = glm::perspective(glm::radians(fov), WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        // Moving the world
        //glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::vec3 direction{};
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
        GLuint mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        // Set the Model matrix in the shader
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw the table
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw the balls
        for (size_t i = 0; i < balls.size(); i++)
            balls[i].Render(glm::vec3(0, i, 0), glm::vec3(0, 0, 0));

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

        // Adjusting minimap aspect ratio
        if (minimapAspect > 1.0f)
            tableHalfLenght *= minimapAspect; // Wider than tall - expand width
        else
            tableHalfWidth /= minimapAspect; // Taller than wide - expand height

        // Padding around the table
        float padding = 0.5f;
        tableHalfLenght += padding;
        tableHalfWidth += padding;

        glm::mat4 topProjection = glm::ortho(-tableHalfWidth, tableHalfWidth, // Left, Right
            -tableHalfLenght, tableHalfLenght, // Bottom, Top
            0.1f, 100.0f);

        glm::mat4 topModel = glm::mat4(1.0f);
        glm::mat4 topView = glm::lookAt(topCamPos, topCamTarget, topCamUp);
        glm::mat4 topMVP = topProjection * topView * topModel;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(topMVP));
        glBindVertexArray(0);

		// Draw the table
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw the balls
        for (size_t i = 0; i < balls.size(); i++)
            balls[i].Render(glm::vec3(0, i, 0), glm::vec3(0, 0, 0));
        glBindVertexArray(0);

        glDisable(GL_SCISSOR_TEST);
        glViewport(0, 0, WIDTH, HEIGHT);

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
