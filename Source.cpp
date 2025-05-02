#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <functional>
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Se tecla '1' premida
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwDestroyWindow(window);
}

using namespace std;

const int RESOLUTION = 86;
const int WIDTH = RESOLUTION * 16, HEIGHT = RESOLUTION * 9;
const char* TITLE = "My 1st P3D Project";
int main(void) {

	GLFWwindow* window = nullptr;
	bool fullScreenMode = false;
	// Runs only one time, in the beggining or when reStated
	function<void()> Start = [&]() {

		if (!glfwInit()) return -1;
		fullScreenMode = false;
		if (fullScreenMode)
			window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, glfwGetPrimaryMonitor(), nullptr); // Full screen
		else
			window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

		if (!window) {

			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, keyCallback);
		double xCursorPos = WIDTH * 0.5, yCursorPos = HEIGHT * 0.5;
		glfwSetCursorPos(window, xCursorPos, yCursorPos);

		};
	// Runs every frame
	function<void()> Update = [&]() {

		// Mouse Callback


		// Keyboard Callback


		// Window Callback
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		glfwPollEvents();
		};
	// The game's Framework
	function<void(function<void()>, function<void()>)> FrameWork = [&](function<void()> Start, function<void()> Update) {

		Start();
		while (!glfwWindowShouldClose(window)) {
			Update();
		}

		glfwDestroyWindow(window);
		glfwTerminate();
		};
	FrameWork(Start, Update);
	return 0;
}
