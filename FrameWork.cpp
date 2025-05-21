#include "FrameWork.h"

void FrameWork::Rendering() {

    // Check if the shader program and VAO are initialized
    if (shaderProgram == 0 || VAO == 0) {
        std::cerr << "Shader program ou VAO não inicializado!" << std::endl;
        return;
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Renderização com shader e MVP
    glUseProgram(shaderProgram);

    // Set up the projection and view matrices
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
};

void FrameWork::RunGame() {

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
