#pragma once
#include "Header.h"

class FrameWork {

protected:

    GLFWwindow* window = nullptr;
    OpenGL_Context* context;

public:

    virtual void Start() = 0;
    virtual void InputManager() = 0;
    virtual void Update(float deltaTime) = 0;

    void Rendering();
    void RunGame();

};
