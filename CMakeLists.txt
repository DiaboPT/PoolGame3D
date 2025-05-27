cmake_minimum_required(VERSION 3.12)
project(PoolGame3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src/header)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/glm)

# Windows specific
if(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/include/windows)
endif()

# Lista de arquivos fonte
set(SOURCES
    src/Main.cpp
    src/common/App.cpp
    src/common/Camera.cpp
    src/common/InputManager.cpp
    src/common/Mesh.cpp
    src/common/Renderer.cpp
    src/common/Shader.cpp
    src/common/Window.cpp
    src/common/ObjModelLoader.cpp
    src/common/OpenGLLoader.cpp
    src/common/ShadersSources.cpp
    src/common/Light.cpp
)

# Add executable
add_executable(PoolGame3D ${SOURCES})

# Link libraries
target_link_libraries(PoolGame3D
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
)

# Windows specific
if(WIN32)
    # Copy DLLs to output directory
    add_custom_command(TARGET PoolGame3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/include/windows/glew32.dll
            ${PROJECT_SOURCE_DIR}/include/windows/glfw3.dll
            $<TARGET_FILE_DIR:PoolGame3D>
    )
endif()