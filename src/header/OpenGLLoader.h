// OpenGLLoader.h
#pragma once

#include "Window.h"

bool LoadOpenGLLibrary();
void* GetOpenGLProcAddress(const char* name);
void UnloadOpenGLLibrary();
bool InitializeOpenGL(PoolGame3D::Window& window);
void ConfigureOpenGL(PoolGame3D::Window& window);
