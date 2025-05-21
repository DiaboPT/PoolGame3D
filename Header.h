#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include <functional>
#include <vector>
#include <string>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

#include "OpenGLHeader.h"
#include "OpenGLLoader.h"
#include "FrameWork.h"

// Set up window title
extern const char* TITLE;
// Set up window size
extern const int WIDTH;
extern const int HEIGHT;
