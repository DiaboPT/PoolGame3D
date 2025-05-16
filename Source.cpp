/* 3D Pool Game
* 
* Summary:
* This work aims to apply the main 3D programming concepts covered in class. Thus, students, organized into
* groups of 4, must develop a program in C++ that, through the use of the OpenGL, GLEW, GLM and GLFW
* libraries, performs the rendering of a 3D scene, applying the respective textures, lighting, and performs an
* animation in response to a keyboard event.This work will result in the delivery, via Moodle, of the files with the
* source code, as well as an oral presentation / defense of the work carried out.
* 
* Objective:
* This work aims to develop a program in C++ (respecting the Object-Oriented Programming paradigm) that renders
* a 3D scene, applying the respective textures and lighting, and performs an animation (movement of one of the
* balls) in response to a keyboard event.
* The requirements are grouped here into 4 steps, in order to guide students in solving the work. Please note,
* however, that you should only submit a single version of your program (the final version), which meets all
* the requirements.
* 
* - Step 1 :
*	• Implement window management and user interface through the GLFW library;
*	• Implement the manipulation of matrices and vectors through the GLM library;
*	• ( 1
*	) Implement the rendering of a parallelepiped (to simulate the pool table) through the OpenGL library
*	(including GLEW);
*	• ( 2 ) The coloring of the parallelepiped fragments must be done so that each face has a distinct color (note
*	that it is not expected that the lighting effects will be implemented at this stage);
*	• The application should allow the user to zoom using the mouse scroll wheel ;
*	• The application should allow navigation (rotation) around the center of the parallelepiped, through mouse
*	movements.
*	• Implement a minimap of the pool table, to be displayed in the top right corner of the application window.
*	This minimap should show a top view perspective of the table. The minimap should not be affected by
*	lighting.
* 
* - Step 2 :
*	• Students must develop a C++ library (respecting the Object-Oriented Programming paradigm) capable of:
*	o Load vertex position data, normals, and texture coordinates from .obj files;
*	o Reorganize vertex information according to face information;
*	o Send vertex data to GPU memory (VAO and respective VBO);
*	o Identify the name of the .mtl file in the .obj file;
*	o Load material properties (.mtl file), including texture image ;
*	o Load the texture image into an OpenGL texture unit;
*	o Provide function for rendering the object.
*	Although the .mtl format supports multiple materials, for this work assume that a .mtl file will only have
*	a single material to apply to the entire object.
*	• The application must use this C++ library to load the models, compose and render a 3D scene that
*	presents 15 balls arranged on a snooker table.
*	Figure 1: Example of composition with billiard table and balls.
*	• The camera should remain still in a position that allows the entire table to be viewed, oriented towards the
*	center of the table.
*	• The library must have its own namespace in which a class (and its respective member functions) will be
*	defined to manage the rendering process. The following member functions must be implemented :
*	o Load(const std::string obj_model_filepath)
*	▪ This function aims to load the data from the .obj file, whose path is passed as an
*	argument, into the CPU memory.
*	▪ You must also load the materials and textures associated with the model into the CPU
*	memory.
*	o Install (void)
*	ENGINEERING IN DIGITAL GAME DEVELOPMENT
*	3D PROGRAMMING – 2ND YEAR Academic Year 2024/2025
*	Polytechnic Institute of Cávado and Ave
*	Higher School of Technology
*	▪ Generates the necessary VAO and VBO and sends the model data (vertices, texture
*	coordinates and normals) to GPU memory.
*	o Render(glm::vec3 position, glm::vec3 orientation)
*	▪ Function to render the model in the position defined by position and orientation defined
*	by orientation .
*	o Functions must also be created that allow the association between the model data (previously
*	loaded to the GPU through the Install() function ) and the attributes of a shader program.
*	o In addition to these mandatory functions, others may be created if deemed necessary.
* 
* - Step 3 :
*	• It should be possible to apply any combination of 4 light sources that fall on the object:
*	o Ambient light;
*	o Directional light;
*	o Spot light;
*	o Conical light.
*	• The parameters of each of the light sources are at the discretion of each group;
*	• The application should allow the user to activate/deactivate each light source, using a key:
*	o '1' – Enable/disable ambient light source;
*	o '2' – Enable/disable directional light source;
*	o '3' – Enable/disable point light source;
*	o '4' – Enable/disable cone light source.
* 
* - Step 4 :
*	• Implement the movement animation (translation and rotation) of one of the billiard balls. This animation
*	should be triggered as soon as the user presses the “Space” key. The animation should stop when the ball
*	hits another billiard ball or the table boundaries.
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

// Function to set up an Object
struct OpenGL_Context {
	// Initialize the member variable  
	string* object_Window_Target = nullptr;
	// 6 faces, 2 triangles/face, 3 vertices/triangle  
	static constexpr GLint numVertices = 6 * 2 * 3;

	// numVertices * xyz/vertice
	GLfloat vertices[numVertices * 3] = {
		// +X (Face #0)
		0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
		// -X (Face #1)
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
		// +Y (Face #2)
		-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		// -Y (Face #3)
		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
		// +Z (Face #4)
		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		// -Z (Face #5)
		0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f
	};
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

		// Checks if OpenGL Library is loaded
		if (!LoadOpenGLLibrary()) {
			std::cerr << "Failed to load OpenGL library!" << std::endl;
			glfwDestroyWindow(window);
			glfwTerminate();
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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		glfwSetKeyCallback(window, keyCallback);

		// Set up the viewport and projection matrix
		glViewport(0, 0, WIDTH, HEIGHT);
		// Set up the projection matrix
		glMatrixMode(GL_PROJECTION);
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

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set up the projection matrix
		glfwSwapBuffers(window);
		// Swap buffers
		glfwPollEvents();
		};

	// Call the framework function
	FrameWork(window, Start, InputManager, Update, Rendering);

	return 0;
}
