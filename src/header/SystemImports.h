#pragma once

#define STB_IMAGE_IMPLEMENTATION 

#ifdef _WIN32
    #include <windows.h>
    #include "OpenGLLoader.h"
    #pragma comment(lib, "glew32.lib")
    #pragma comment(lib, "glfw3dll.lib")
    #pragma comment(lib, "opengl32.lib")

    // Exporta variáveis para otimização de GPU em notebooks
    extern "C" {
        __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
        __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
        __declspec(dllexport) DWORD IntelGraphicsRequestHighPerformance = 0x00000001;
    }
#else
    // MacOS/Linux: não precisa desses includes/pragmas
    // Função dummy para manter compatibilidade
    inline bool LoadOpenGLLibrary() { return true; }
#endif