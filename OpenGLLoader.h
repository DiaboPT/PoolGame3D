// OpenGLLoader.h
#pragma once

bool LoadOpenGLLibrary();
void UnloadOpenGLLibrary();
void* GetOpenGLProcAddress(const char* name);
