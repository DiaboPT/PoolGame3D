// OpenGLLoader.h
#pragma once

bool LoadOpenGLLibrary();
void* GetOpenGLProcAddress(const char* name);
void UnloadOpenGLLibrary();
