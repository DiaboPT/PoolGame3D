// OpenGLLoader.cpp
#include "OpenGLLoader.h"

// Declaração da variável glLib fora dos blocos condicionais
#ifdef _WIN32
#include <windows.h>
static HMODULE glLib = nullptr;
#elif __linux__
#include <dlfcn.h>
static void* glLib = nullptr;
#else
// Para macOS, não precisamos de um tipo específico
static void* glLib = nullptr;
#endif

bool LoadOpenGLLibrary() {
#ifdef _WIN32
    glLib = LoadLibraryA("opengl32.dll");
    return glLib != nullptr;
#elif __linux__
    glLib = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
    return glLib != nullptr;
#else
    // No macOS, OpenGL já está disponível nativamente
    return true;
#endif
}

void* GetOpenGLProcAddress(const char* name) {
#ifdef _WIN32
    void* proc = (void*)wglGetProcAddress(name);
    if (!proc && glLib)
        proc = (void*)GetProcAddress(glLib, name);
    return proc;
#elif __linux__
    if (!glLib) return nullptr;
    return dlsym(glLib, name);
#else
    // No macOS, OpenGL já está disponível nativamente
    return nullptr;
#endif
}

void UnloadOpenGLLibrary() {
#ifdef _WIN32
    if (glLib) FreeLibrary(glLib);
#elif __linux__
    if (glLib) dlclose(glLib);
#endif
    glLib = nullptr;
}
