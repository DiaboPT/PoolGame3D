#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Se tecla '1' premida
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwDestroyWindow(window);
}

int main(void) {

	// Variables
	GLFWwindow* window;

	// Window resolution
	const int res = 86;
	int width = res * 16, height = res * 9;

	bool fullScreenMode = false;

	// Start
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(width, height, "My 1st P3D Project", nullptr, nullptr);

	if (fullScreenMode) window = glfwCreateWindow(width, height, "My 1st P3D Project", glfwGetPrimaryMonitor(), nullptr); // Full screen

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	double xCursorPos = width * 0.5, yCursorPos = height * 0.5;
	glfwSetCursorPos(window, xCursorPos, yCursorPos);

	// Update
	while (!glfwWindowShouldClose(window)) {

		// Mouse Callback


		// Keyboard Callback


		// Window Callback
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}