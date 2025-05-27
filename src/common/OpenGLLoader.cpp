// OpenGLLoader.cpp
#include "OpenGLLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"

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

bool InitializeOpenGL(PoolGame3D::Window& window) {
    glfwMakeContextCurrent(window.getHandle());
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Falha ao inicializar GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "GLEW inicializado com sucesso!" << std::endl;
    std::cout << "Versão do OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor do OpenGL: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer do OpenGL: " << glGetString(GL_RENDERER) << std::endl;
    // Verificar funções OpenGL
    if (!glfwGetProcAddress("glGenVertexArrays") ||
        !glfwGetProcAddress("glBindVertexArray") ||
        !glfwGetProcAddress("glGenBuffers")) {
        std::cerr << "Funções OpenGL não disponíveis!" << std::endl;
        return false;
    }
    if (!LoadOpenGLLibrary()) {
        std::cerr << "Failed to load OpenGL library!" << std::endl;
        return false;
    }
    std::cout << "OpenGL OK." << std::endl;
    return true;
}

void ConfigureOpenGL(PoolGame3D::Window& window) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, window.getWidth(), window.getHeight());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
