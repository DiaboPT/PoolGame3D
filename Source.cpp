#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <functional>
#include "OpenGLLoader.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

using namespace std;

// The State of OpenGL
struct OpenGL_Context {

	string* object_Window_Target;
	float vertices[9]{
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
	};

};

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Se tecla 'Escape' premida
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

constexpr int RESOLUTION = 86;
constexpr int WIDTH = RESOLUTION * 16, HEIGHT = RESOLUTION * 9;
const char* TITLE = "My 1st P3D Project";
int main(void) {

	GLFWwindow* window = nullptr;
	bool fullScreenMode = false;
	// Runs only one time, in the beggining or when reStated
	function<void()> Start = [&]() {

		if (!LoadOpenGLLibrary()) {
			std::cerr << "Failed to load OpenGL library!" << std::endl;
			exit(1);
		}

		void* glClearPtr = GetOpenGLProcAddress("glClear");
		if (!glClearPtr) {
			std::cerr << "Failed to load glClear!" << std::endl;
		}

		if (!glfwInit()) return -1;
		window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			std::cerr << "Failed to initialize GLEW" << std::endl;
			return -1;
		}

		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPos(window, WIDTH * 0.5, HEIGHT * 0.5);

		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glm::mat4 model = glm::mat4(1.0f); // Identidade (sem transformações ainda)

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 3.0f, 5.0f), // posição da câmara
			glm::vec3(0.0f, 0.0f, 0.0f), // olha para o centro
			glm::vec3(0.0f, 1.0f, 0.0f)  // vetor "cima"
		);

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),          // campo de visão
			(float)WIDTH / (float)HEIGHT, // aspeto
			0.1f, 100.0f                  // plano próximo e distante
		);

		};
	// Inputs
	function<void()> InputManager = [&]() {
		};
	// Runs every frame
	function<void()> Update = [&]() {
		};
	// Renders
	function<void()> Rendering = [&]() {

		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		glfwPollEvents();
		};

	// The game's Framework
	const function<void(
		function<void()>,
		function<void()>,
		function<void()>,
		function<void()>
		)> FrameWork = [&](
			function<void()> Start,
			function<void()> InputManager,
			function<void()> Update,
			function<void()> Rendering) {

		Start();
		while (!glfwWindowShouldClose(window)) {

			InputManager();
			Update();
			Rendering();
		}
		glfwDestroyWindow(window);
		glfwTerminate();
		};
	FrameWork(Start, InputManager, Update, Rendering);
	return 0;
}
